#include "Base.h"
#include "MeshBase.h"


namespace Game
{

	MeshBase::MeshBase(const Ogre::String &objectName, const Ogre::String &fileName)
	{
		m_sceneNode = Base::Instance().GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		m_entity = Base::Instance().GetSceneManager()->createEntity(objectName, fileName);
		m_sceneNode->attachObject(m_entity);
		m_entity->setNormaliseNormals(true);
	}
	
	MeshBase::MeshBase(const Ogre::String &objectName,const Ogre::String &fileName,Ogre::SceneNode *parentNode)
	{
		m_sceneNode = parentNode->createChildSceneNode(objectName);
		m_entity = Base::Instance().GetSceneManager()->createEntity(objectName, fileName);
		m_sceneNode->attachObject(m_entity);
		m_entity->setNormaliseNormals(true);
	}
	
	MeshBase::~MeshBase()
	{
	}

}