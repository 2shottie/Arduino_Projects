# 1 "C:\\Users\\cody\\Documents\\Arduino PLC IDE\\Machine_IO\\LLSketch\\LLSketch.ino"
# 2 "C:\\Users\\cody\\Documents\\Arduino PLC IDE\\Machine_IO\\LLSketch\\LLSketch.ino" 2

/* opta_1.0.3
*/

struct PLCSharedVarsInput_t
{
};
PLCSharedVarsInput_t& PLCIn = (PLCSharedVarsInput_t&)m_PLCSharedVarsInputBuf;

struct PLCSharedVarsOutput_t
{
};
PLCSharedVarsOutput_t& PLCOut = (PLCSharedVarsOutput_t&)m_PLCSharedVarsOutputBuf;


AlPlc AxelPLC(-1340532227);

// shared variables can be accessed with PLCIn.varname and PLCOut.varname

/*
// Enable usage of EtherClass, to set static IP address and other
#include <PortentaEthernet.h>
arduino::EthernetClass eth(&m_netInterface);
*/

void setup()
{
/*
	// Configure static IP address
	IPAddress ip(192, 168, 2, 4);
	IPAddress dns(8, 8, 8, 8);
	IPAddress gateway(192, 168, 2, 1);
	IPAddress subnet(255, 255, 255, 0);
	// If cable is not connected this will block the start of PLC with about 60s of timeout!
	eth.begin(ip, dns, gateway, subnet);
*/

 AxelPLC.Run();
}

void loop()
{

}
