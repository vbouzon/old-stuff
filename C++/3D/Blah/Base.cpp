#include "Base.h"

Base& Base::Instance()
{
	static Base instance;
	return instance;
}

Base::Base()
{
	m_root = NULL;
	m_window = NULL;
}

Base::~Base()
{
	if(m_window)
	{
		m_window->destroy();
	}
	if(m_root) delete m_root;
}

void Base::RequestShutdown()
{
	m_run = false;
}

void Base::Shutdown()
{
	Game::StateManager::Instance().Terminate();
	Game::InputManager::Instance().Terminate();
}

Ogre::RenderWindow* Base::GetWindow()
{
	return m_window;
}

Ogre::Root* Base::GetRoot()
{
	return m_root;
}

bool Base::Run()
{
	MSG  msg; 
	if(!Init())
	{
		return false;
	}
	while(m_run)
	{

		t = time.getMilliseconds();
		t = t / 1000.0f;
		time.reset();
		while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) 
		{ 
			TranslateMessage( &msg ); 
			DispatchMessage( &msg ); 
		} 
		Update(t);
		Render();
		ShowStats();
	}
	
	Shutdown();
	
	return true;
}

void Base::ShowStats()
{
	static Ogre::String currFps = "Current FPS: ";
	static Ogre::String avgFps = "Average FPS: ";
	static Ogre::String bestFps = "Meilleur FPS: ";
	static Ogre::String worstFps = "Pire FPS: ";
	static Ogre::String tris = "Triangle Count: ";
	
	Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
	Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
	Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
	Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
		
	const Ogre::RenderTarget::FrameStats& stats = m_window->getStatistics();

	guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.lastFPS));
	guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
	guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS)+" "+Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
	guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS)+" "+Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

	Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
	guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));
}

bool Base::Init()
{
	m_run = true;

	m_root = new Ogre::Root("Plugins.cfg","Config.cfg","Ogre.log");

	InitResources();

	if (!Configure()) return false;

	Game::InputManager::Instance().Init(*m_window);
	
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(30);
	
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Game::StateManager::Instance().Init();

	m_window->resetStatistics();

	mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	mDebugOverlay->show();

	return true;
}

bool Base::Update(float t)
{
	Game::InputManager::Instance().Update(t);
	Game::StateManager::Instance().Update(t);
	return true;
}


bool Base::Render()
{
	m_root->renderOneFrame();
	
	return true;
}

void Base::InitResources()
{
	Ogre::ConfigFile cf;
	cf.load("Resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}

bool Base::Configure()
{
	if(m_root->showConfigDialog())
	{
		m_window = m_root->initialise(true, "fuck d4 w0rld");
	}
	else
	{
		RequestShutdown();
		return false;
	}

	return true;
}



