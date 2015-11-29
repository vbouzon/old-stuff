#ifndef MESHBASE_H
#define MESHBASE_H

namespace Game
{

	class MeshBase
	{
	public:

		MeshBase(const Ogre::String &objectName, const Ogre::String &fileName);
		MeshBase(const Ogre::String &objectName,const Ogre::String &fileName,Ogre::SceneNode *parentNode);
		virtual ~MeshBase();

		virtual void Update(float t) = 0;

		inline Ogre::SceneNode *GetSceneNode(){return m_sceneNode;}
		inline Ogre::Entity *GetEntity(){return m_entity;}

	private:

		Ogre::SceneNode *m_sceneNode;
		Ogre::Entity *m_entity;
	};

}

#endif