using Aurora;
using Aurora.Devices;
using Aurora.Settings;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO.Ports;

public class ESP8266 {

    public string devicename = "ESP8266";
    public bool enabled = true;


    private VariableRegistry default_registry;

    private Stopwatch redrawWatch = new Stopwatch();

    private static UdpClient udp;
    private AConnection[] connections;
    private static DeviceKeys devicekey;
    private static bool autoSwitch;

    private static bool wifiEnable;
    private static int tcpPort;
    private static int udpPort;

    private static bool serialEnable;
    private static int serialBaud;

    private Color lastColor;

    private uint lastUpdateTime = uint.MaxValue;

    public string GetDeviceName() {
        return devicename;
    }

    public bool Initialize() {
        devicekey = Global.Configuration.VarRegistry.GetVariable<DeviceKeys>("esp8266_devicekey");
        autoSwitch = Global.Configuration.VarRegistry.GetVariable<bool>("esp8266_auto_switch");
        wifiEnable = Global.Configuration.VarRegistry.GetVariable<bool>("esp8266_wifi_enable");
        var wifiConnections = new AConnection[0];
        if (wifiEnable)
        {
            udp = new UdpClient();
            tcpPort = Global.Configuration.VarRegistry.GetVariable<int>("esp8266_wifi_port_tcp");
            udpPort = Global.Configuration.VarRegistry.GetVariable<int>("esp8266_wifi_port_udp");

            var ips = Global.Configuration.VarRegistry.GetVariable<string>("esp8266_wifi").Split(',');

            wifiConnections = new AConnection[ips.Length];
            for (var i = 0; i < ips.Length; i++)
            {
                wifiConnections[i] = new WiFiConnection(ips[i]);
            }
        }
        serialEnable = Global.Configuration.VarRegistry.GetVariable<bool>("esp8266_com_enable");
        var serialConnections = new AConnection[0];
        if (serialEnable)
        {
            serialBaud = Global.Configuration.VarRegistry.GetVariable<int>("esp8266_com_baud");

            var ports = Global.Configuration.VarRegistry.GetVariable<string>("esp8266_com").Split(',');

            serialConnections = new AConnection[ports.Length];
            for (var i = 0; i < ports.Length; i++)
            {
                serialConnections[i] = new SerialConnection(ports[i]);
            }
        }

        connections = new AConnection[wifiConnections.Length + serialConnections.Length];
        wifiConnections.CopyTo(connections, 0);
        serialConnections.CopyTo(connections, wifiConnections.Length);

        lastColor = Color.Black;
        redrawWatch.Start();
        return true;
    }

    public void Shutdown()
    {
        Mode restoreMode = Global.Configuration.VarRegistry.GetVariable<Mode>("esp8266_restore_mode");
        foreach (var connection in connections) {
            connection.executeCommand(new byte[] { (byte)Command.SetMode, (byte)restoreMode });
        }
        udp = null;
        connections = null;
    }

    public void Reset()
    {
        Shutdown();
        Initialize();
    }

    public bool Reconnect() { return true; } //That's done automatically

    public bool UpdateDevice(Dictionary<DeviceKeys, Color> keyColors, bool forced = false)
    {
        if (keyColors.ContainsKey(devicekey))
        {
            var c = keyColors[devicekey];
            if (redrawWatch.ElapsedMilliseconds < 10000)
            {
                if (c == lastColor && !forced) return true;
            } else
            {
                redrawWatch.Restart();
            }
            lastColor = c;

            foreach (var connection in connections)
            {
                connection.sendColors(new byte[] { c.R, c.G, c.B });
            }
        }
        return true;
    }

    public VariableRegistry GetRegisteredVariables() {
        if (default_registry == null)
        {
            default_registry = new VariableRegistry();
            default_registry.Register("esp8266_devicekey", DeviceKeys.Peripheral, "Key to Use", DeviceKeys.MOUSEPADLIGHT15, DeviceKeys.Peripheral);
            default_registry.Register("esp8266_wifi_enable", false, "Use WiFi");
            default_registry.Register("esp8266_wifi", "", "ESP8266 IP Adresses (Comma separated)");
            default_registry.Register("esp8266_wifi_port_tcp", 1337, "ESP8266 TCP Port");
            default_registry.Register("esp8266_wifi_port_udp", 1337, "ESP8266 UDP Port");
            default_registry.Register("esp8266_com_enable", false, "Use Serial Port (USB)");
            default_registry.Register("esp8266_com", "", "Serial Ports (Comma separated)");
            default_registry.Register("esp8266_com_baud", 74880, "Baud Rate", 0, 2000000);
            default_registry.Register("esp8266_auto_switch", true, "Automatically switch to Aurora Mode");
            default_registry.Register("esp8266_restore_mode", Mode.DefaultMode, "Mode to return to when disconnecting");
        }
        return default_registry;
    }

    private abstract class AConnection
    {
        private Stopwatch checkWatch = new Stopwatch();
        private bool online;

        public abstract bool executeCommand(byte[] payload);
        public abstract void sendColors(byte[] payload);

        public bool isOnline()
        {
            if (checkWatch.ElapsedMilliseconds > 1000) //Only test every 10 seconds
            {
                checkOnline();
            }
            return online;
        }

        public void checkOnline()
        {
            checkWatch.Restart();
            if (autoSwitch)
            {
                online = this.executeCommand(new byte[] { (byte)Command.SetMode, (byte)Mode.Aurora }); //setMode aurora, called here to set mode on reconnect/crash
            }
            else
            {
                online = this.executeCommand(new byte[] { (byte)Command.Heartbeat }); //heartbeat
            }
        }
    }

    private class WiFiConnection : AConnection
    {
        private string ip;

        public WiFiConnection(string _ip)
        {
            ip = _ip;
            checkOnline();
        }

        public override bool executeCommand(byte[] payload)
        {
            try
            {
                var tcp = new TcpClient();
                tcp.SendTimeout = 100;
                tcp.ReceiveTimeout = 100;
                tcp.Connect(ip, tcpPort);
                var stream = tcp.GetStream();

                stream.Write(payload, 0, payload.Length);
                var response = new byte[1];
                stream.Read(response, 0, 1);

                var buffer = new byte[255];
                while (stream.Read(buffer, 0, 255) != 0) { } //Read everything

                stream.Close();
                tcp.Close();
                return response[0] == 0x53; //S
            }
            catch (Exception)
            {
                return false;
            }
        }

        public override void sendColors(byte[] payload)
        {
            if (isOnline()) {
                udp.SendAsync(payload, payload.Length, ip, udpPort);
            }
        }
    }

    private class SerialConnection : AConnection
    {
        private SerialPort port;

        public SerialConnection(String _port)
        {
            port = new SerialPort(_port, serialBaud);
        }

        public override bool executeCommand(byte[] payload)
        {
            var pl = new byte[payload.Length + 1];
            pl[0] = (byte)payload.Length;
            payload.CopyTo(pl, 1);
            try
            {
                port.Open();
                port.Write(pl, 0, pl.Length);

                var response = new byte[1];
                port.Read(response, 0, 1);

                var buffer = new byte[255];
                while (port.Read(buffer, 0, 255) != 0) { } //Read everything

                port.Close();
                return response[0] == 0x53; //S
            } catch (Exception)
            {
                return false;
            }
        }

        public override void sendColors(byte[] payload)
        {
            if (isOnline())
            {
                var command = new byte[payload.Length + 1];
                command[0] = 0;
                payload.CopyTo(command, 1);
                executeCommand(command);
            }
        }
    }

    private enum Command
    {
        RawData = 0x0,
        SetMode = 0x1,
        SetSolidColor = 0x2,
        ReadSettings = 0x3,
        WriteSettings = 0x4,
        Heartbeat = 0xFE,
        Reset = 0xFF
    }

    private enum Mode
    {
        SolidColor = 0x00,
        ColorFlow = 0x01,
        Aurora = 0x02,
        DefaultMode = 0xFF
    }
}