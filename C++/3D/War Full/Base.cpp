#include "Base.h"

Base& Base::Instance()
{
	static Base instance;
	return instance;
}

Base::Base()
{
	m_root = NULL;
	m_scene_manager = NULL;
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

Ogre::SceneManager* Base::GetSceneManager()
{
	return m_scene_manager;
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
	
	// update stats when necessary
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

	// Cree le root
	#ifdef _DEBUG
	m_root = new Ogre::Root("Plugins_d.cfg","Config_d.cfg","Ogre_d.log");
	#else
	m_root = new Ogre::Root("plugins.cfg","config.cfg","ogre.log");
	#endif

	InitResources();

	if (!Configure()) return false;

	m_scene_manager = m_root->createSceneManager(Ogre::ST_GENERIC);

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
	Game::StateManager::Instance().Update(t);
	return true;
}


bool Base::Render()
{
	m_root->_fireFrameStarted();
	m_window->update();
	m_root->_fireFrameEnded(); 
		
	return true;
}

void Base::InitResources()
{
	Ogre::ConfigFile cf;
	#ifdef _DEBUG
	cf.load("resources_d.cfg");
	#else
	cf.load("resources.cfg");
	#endif

	Ogre::ConfigFile::SettingsIterator i = cf.getSettingsIterator();
	Ogre::String typeName, archName;
	while (i.hasMoreElements())
	{
		typeName = i.peekNextKey();
		archName = i.getNext();
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, "General" );
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



