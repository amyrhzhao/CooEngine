#include <Coo/Inc/Coo.h>
using namespace Coo::Core;
using namespace Coo::Network;
class Tank
{
public:
	RTTI(Tank)

	void Serialize(StreamWriter& sw)
	{
		sw.Write(player);
		sw.Write(hp);
		sw.Write(armor);
		sw.Write(speed);
		sw.Write(damage);
	}
	void Deserialize(StreamReader& sr)
	{
		sr.Read(player);
		sr.Read(hp);
		sr.Read(armor);
		sr.Read(speed);
		sr.Read(damage);
	}

	std::string player{ "" };
	int hp{ 0 };
	int armor{ 0 };
	float speed{ 0 };
	float damage{ 0 };
};

int main(int argc, char* argv[])
{
	LOG("Hello");
	int outI = 42;
	float outF = 3.14f;
	double outD = 6.28;
	char outC = 'a';
	bool outB = true;
	std::string outStr = "test";

	MemoryStream outStream;
	StreamWriter writer(outStream);
	writer.Write(outI);
	writer.Write(outF);
	writer.Write(outD);
	writer.Write(outC);
	writer.Write(outB);
	writer.Write(outStr);

	int inI = 0;
	float inF = 0.0f;
	double inD = 0.0;
	char inC = '\0';
	bool inB = false;
	std::string inStr = "garbage";

	MemoryStream inStream(outStream.GetData(), outStream.GetSize());
	StreamReader reader(inStream);
	reader.Read(inI);
	reader.Read(inF);
	reader.Read(inD);
	reader.Read(inC);
	reader.Read(inB);
	reader.Read(inStr);

	ASSERT(inI == outI, "Input/output data mismatch");
	ASSERT(inF == outF, "Input/output data mismatch");
	ASSERT(inD == outD, "Input/output data mismatch");
	ASSERT(inC == outC, "Input/output data mismatch");
	ASSERT(inB == outB, "Input/output data mismatch");
	ASSERT(inStr == outStr, "Input/output data mismatch");

	Tank myTank;
	myTank.player = "Peter";
	myTank.hp = 77;
	myTank.armor = 156;
	myTank.speed = 23.0f;
	myTank.damage = 74.0f;

	MemoryStream tankOut;
	StreamWriter tankWriter(tankOut);
	myTank.Serialize(tankWriter);

	MemoryStream tankIn(tankOut.GetData(), tankOut.GetSize());
	StreamReader tankReader(tankIn);

	Tank urTank;
	urTank.Deserialize(tankReader);

	ASSERT(myTank.player == urTank.player, "Not the same tank!");
	ASSERT(myTank.hp == urTank.hp, "Not the same tank!");
	ASSERT(myTank.armor == urTank.armor, "Not the same tank!");
	ASSERT(myTank.speed == urTank.speed, "Not the same tank!");
	ASSERT(myTank.damage == urTank.damage, "Not the same tank!");

	//----LinkingContext and ObjectFactory Test-----
	ObjectFactory factory;
	factory.Register<Tank>();

	Tank* tank = factory.CreateInstance<Tank>();

	LinkingContext linkingContext;
	int tankId = linkingContext.GetNetworkId(tank);

	ASSERT(tankId == LinkingContext::InvalidId, "Unexpected id for unregistered object.");

	tankId = linkingContext.Register(tank);

	ASSERT(tankId != LinkingContext::InvalidId, "Object not registered with linking context.");

	linkingContext.Unregister(tank);
	delete tank;
	return 0;
}
