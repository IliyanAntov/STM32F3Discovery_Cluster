using System;
using System.Timers;
using vxlapi_NET;
using static vxlapi_NET.XLClass;
using static vxlapi_NET.XLDefine;

namespace Cluster
{
    public class CANManager
    {
        private static XLDriver CANDriver = new XLDriver();
        private static String appName = "CAN";

        // Driver configuration
        private static XLClass.xl_driver_config driverConfig = new XLClass.xl_driver_config();

        // Variables required by XLDriver
        private static XLDefine.XL_HardwareType hwType = XLDefine.XL_HardwareType.XL_HWTYPE_NONE;
        private static uint hwIndex = 0;
        private static uint hwChannel = 0;
        private static int portHandle = -1;
        private static UInt64 accessMask = 0;
        private static UInt64 permissionMask = 0;
        private static UInt64 txMask = 0;
        private static UInt64 rxMask = 0;
        private static int txCi = -1;
        private static int rxCi = -1;
        private static uint mask = 0;
        private static int speed = 0;
        private static int horn = 0;
        private static int curr_time_hours = 0;
        private static int curr_time_minutes = 0;

        public CANManager()
        {
            InitializeCAN();
        }

        public void InitializeCAN()
        {
            XL_Status status;

            // Open XL Driver
            status = CANDriver.XL_OpenDriver();

            status = CANDriver.XL_GetDriverConfig(ref driverConfig);

            while (!GetAppChannelAndTestIsOk(1, ref txMask, ref txCi) || !GetAppChannelAndTestIsOk(0, ref rxMask, ref rxCi))
            {
                PrintAssignErrorAndPopupHwConf();
            }

            accessMask = txMask | rxMask;
            permissionMask = accessMask;

            // Open port
            status = CANDriver.XL_OpenPort(ref portHandle, appName, accessMask, ref permissionMask, 1024, XLDefine.XL_InterfaceVersion.XL_INTERFACE_VERSION, XLDefine.XL_BusTypes.XL_BUS_TYPE_CAN);
            Console.WriteLine("\n\nOpen Port             : " + status);
            if (status != XL_Status.XL_SUCCESS) PrintFunctionError();

            // Check port
            status = CANDriver.XL_CanRequestChipState(portHandle, accessMask);
            Console.WriteLine("Can Request Chip State: " + status);
            if (status != XL_Status.XL_SUCCESS) PrintFunctionError();

            // Activate channel
            status = CANDriver.XL_ActivateChannel(portHandle, accessMask, XLDefine.XL_BusTypes.XL_BUS_TYPE_CAN, XLDefine.XL_AC_Flags.XL_ACTIVATE_NONE);
            Console.WriteLine("Activate Channel      : " + status);
            if (status != XL_Status.XL_SUCCESS) PrintFunctionError();

            Console.WriteLine("Set Notification      : " + status);
            if (status != XL_Status.XL_SUCCESS) PrintFunctionError();

            status = CANDriver.XL_ResetClock(portHandle);
            Console.WriteLine("Reset Clock           : " + status + "\n\n");
            if (status != XL_Status.XL_SUCCESS) PrintFunctionError();
        }

        private static int PrintFunctionError()
        {
            Console.WriteLine("\nERROR: Function call failed!");
            return -1;
        }

        private static void PrintAssignErrorAndPopupHwConf()
        {
            Console.WriteLine("\nPlease check application settings of \"" + appName + " CAN1/CAN2\",\nassign them to available hardware channels and press enter.");
            CANDriver.XL_PopupHwConfig();
            Console.ReadKey();
        }

        private static bool GetAppChannelAndTestIsOk(uint appChIdx, ref UInt64 chMask, ref int chIdx)
        {
            XL_Status status = CANDriver.XL_GetApplConfig(appName, appChIdx, ref hwType, ref hwIndex, ref hwChannel, XL_BusTypes.XL_BUS_TYPE_CAN);
            if (status != XLDefine.XL_Status.XL_SUCCESS)
            {
                Console.WriteLine("XL_GetApplConfig      : " + status);
                PrintFunctionError();
            }

            chMask = CANDriver.XL_GetChannelMask(hwType, (int)hwIndex, (int)hwChannel);
            chIdx = CANDriver.XL_GetChannelIndex(hwType, (int)hwIndex, (int)hwChannel);
            if (chIdx < 0 || chIdx >= driverConfig.channelCount)
            {
                // the (hwType, hwIndex, hwChannel) triplet stored in the application configuration does not refer to any available channel.
                return false;
            }

            // test if CAN is available on this channel
            return (driverConfig.channel[chIdx].channelBusCapabilities & XL_BusCapabilities.XL_BUS_ACTIVE_CAP_CAN) != 0;
        }

        public static void TimerHandler(Object source, ElapsedEventArgs e)
        {
            SendInfo(mask, speed);
        }

        public static void SetInfo(uint new_mask, int new_speed, int new_horn, int curr_hour, int curr_min)
        {
            mask = new_mask;
            speed = new_speed;
            horn = new_horn;
            curr_time_hours = curr_hour;
            curr_time_minutes = curr_min;
        }

        public static void SendInfo(uint mask, int speed)
        {
            XLDefine.XL_Status txStatus;

            xl_event_collection xlEventCollection = new xl_event_collection(4);

            xlEventCollection.xlEvent[0].tagData.can_Msg.id = 30;
            xlEventCollection.xlEvent[0].tagData.can_Msg.dlc = 1;
            xlEventCollection.xlEvent[0].tagData.can_Msg.data[0] = (byte)mask;
            xlEventCollection.xlEvent[0].tag = XL_EventTags.XL_TRANSMIT_MSG;

            xlEventCollection.xlEvent[1].tagData.can_Msg.id = 10;
            xlEventCollection.xlEvent[1].tagData.can_Msg.dlc = 1;
            xlEventCollection.xlEvent[1].tagData.can_Msg.data[0] = (byte)speed;
            xlEventCollection.xlEvent[1].tag = XL_EventTags.XL_TRANSMIT_MSG;

            xlEventCollection.xlEvent[2].tagData.can_Msg.id = 20;
            xlEventCollection.xlEvent[2].tagData.can_Msg.dlc = 1;
            xlEventCollection.xlEvent[2].tagData.can_Msg.data[0] = (byte)horn;
            xlEventCollection.xlEvent[2].tag = XL_EventTags.XL_TRANSMIT_MSG;

            xlEventCollection.xlEvent[3].tagData.can_Msg.id = 40;
            xlEventCollection.xlEvent[3].tagData.can_Msg.dlc = 2;
            xlEventCollection.xlEvent[3].tagData.can_Msg.data[0] = (byte)curr_time_hours;
            xlEventCollection.xlEvent[3].tagData.can_Msg.data[1] = (byte)curr_time_minutes;
            xlEventCollection.xlEvent[3].tag = XL_EventTags.XL_TRANSMIT_MSG;

            txStatus = CANDriver.XL_CanTransmit(portHandle, txMask, xlEventCollection);
        }

        public static void ResetCluster()
        {
            XLDefine.XL_Status txStatus;

            xl_event_collection xlEventCollection = new xl_event_collection(4);

            xlEventCollection.xlEvent[0].tagData.can_Msg.id = 30;
            xlEventCollection.xlEvent[0].tagData.can_Msg.dlc = 1;
            xlEventCollection.xlEvent[0].tagData.can_Msg.data[0] = 0;
            xlEventCollection.xlEvent[0].tag = XL_EventTags.XL_TRANSMIT_MSG;

            xlEventCollection.xlEvent[1].tagData.can_Msg.id = 10;
            xlEventCollection.xlEvent[1].tagData.can_Msg.dlc = 1;
            xlEventCollection.xlEvent[1].tagData.can_Msg.data[0] = 0;
            xlEventCollection.xlEvent[1].tag = XL_EventTags.XL_TRANSMIT_MSG;

            xlEventCollection.xlEvent[2].tagData.can_Msg.id = 20;
            xlEventCollection.xlEvent[2].tagData.can_Msg.dlc = 1;
            xlEventCollection.xlEvent[2].tagData.can_Msg.data[0] = 0;
            xlEventCollection.xlEvent[2].tag = XL_EventTags.XL_TRANSMIT_MSG;

            xlEventCollection.xlEvent[3].tagData.can_Msg.id = 40;
            xlEventCollection.xlEvent[3].tagData.can_Msg.dlc = 2;
            xlEventCollection.xlEvent[3].tagData.can_Msg.data[0] = 0;
            xlEventCollection.xlEvent[3].tagData.can_Msg.data[1] = 0;
            xlEventCollection.xlEvent[3].tag = XL_EventTags.XL_TRANSMIT_MSG;


            txStatus = CANDriver.XL_CanTransmit(portHandle, txMask, xlEventCollection);
        }
    }
}
