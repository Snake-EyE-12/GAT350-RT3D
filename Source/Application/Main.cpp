#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Core/StringUtils.h"

using namespace std;

int main(int argc, char* argv[])
{
	std::string testString = "This Is a 45 numbered STRING";
	std::string ts = "This Is a LONGER 92 numbered STRING";
	std::cout << nc::StringUtils::CreateUnique(testString);
	std::cout << nc::StringUtils::CreateUnique(testString);
	std::cout << nc::StringUtils::CreateUnique(testString);
	std::cout << nc::StringUtils::CreateUnique(testString);
	std::cout << nc::StringUtils::CreateUnique(testString) << endl;
	std::cout << nc::StringUtils::ToUpper(testString) << endl;
	std::cout << nc::StringUtils::ToLower(testString) << endl;
	std::cout << nc::StringUtils::IsEqualIgnoreCase(testString, nc::StringUtils::ToLower(ts)) << endl;
	/*
	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");

	ENGINE.Initialize();

	auto world = make_unique<nc::World05>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();
	*/

	return 0;
}
