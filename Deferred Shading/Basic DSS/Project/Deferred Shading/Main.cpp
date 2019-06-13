
//http://www.ogre3d.org/forums/viewtopic.php?f=5&t=74629&p=475862


#include "DeferredShadingSystem.h"
#include <OIS.h>


float GetRandomColorValue(void)
{
	int value = rand();

	if (value > 255)
		return GetRandomColorValue();

	return value / 255.f;
}


Ogre::ColourValue GetRandomColor(void)
{
	return Ogre::ColourValue(GetRandomColorValue(), GetRandomColorValue(), GetRandomColorValue());
}


void main(void)
{
	Ogre::Root *graphic = new Ogre::Root("");

	graphic->loadPlugin("Plugin_CgProgramManager_d");
	graphic->loadPlugin("RenderSystem_Direct3D9_d");

	if (!graphic->showConfigDialog())
		return;

	Ogre::RenderWindow *window = graphic->initialise(true, "Deferred Shading");

	size_t handle;
	window->getCustomAttribute("WINDOW", &handle);

	OIS::InputManager *inputManager = OIS::InputManager::createInputSystem((size_t)handle);
	OIS::Keyboard *keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
	OIS::Mouse *mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));

	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("DeferredShading/PostProcess/", "FileSystem");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("DeferredShading/Texture/", "FileSystem");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("DeferredShading/", "FileSystem");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/", "FileSystem");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Ogre::SceneManager *scene = graphic->createSceneManager(Ogre::ST_GENERIC);
	scene->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));

	//scene->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE);
	scene->setShadowTextureCasterMaterial("DeferredShading/Shadows/Caster");
	scene->setShadowFarDistance(150);
	scene->setShadowTextureSize(512);
	scene->setShadowTextureCount(1);
	scene->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
	scene->setShadowDirectionalLightExtrusionDistance(75);

	Ogre::Camera *camera = scene->createCamera("Camera");
	camera->setPosition(25, 5, 0);
	//camera->setPosition(100, 100, 100);
	camera->lookAt(0, 0, 0);
	camera->setNearClipDistance(.01f);
	camera->setFarClipDistance(1000);
	camera->setAutoAspectRatio(true);

	Ogre::SceneNode *sibenik = scene->getRootSceneNode()->createChildSceneNode("sibenik");
	sibenik->attachObject(scene->createEntity("sibenik.mesh"));

	Ogre::SceneNode *headNode = scene->getRootSceneNode()->createChildSceneNode("headNode");
	headNode->attachObject(scene->createEntity("OgreHead.mesh"));
	headNode->setPosition(Ogre::Vector3(-8.5, 4.5, 0));
	//headNode->setPosition(Ogre::Vector3(0, 0, 0));
	headNode->setScale(Ogre::Vector3(0.09f));
	headNode->yaw(Ogre::Degree(90));

	Ogre::Viewport *viewport = window->addViewport(camera);

	DeferredShadingSystem *dss = new DeferredShadingSystem(viewport);
	//dss->CreateDirectionalLight()->GetOgreLight()->setCastShadows(true);

	// Do some animation for node a-f
	// Generate helix structure
	float seconds_per_station = 1.0f;
	float r = 1.0;
	Ogre::Vector3 base(-4.75, 1.5, -2);

	float h=3;
	const size_t s_to_top = 16;
	const size_t stations = s_to_top*2-1;
	float ascend = h/((float)s_to_top);
	float stations_per_revolution = 1.5f;
	size_t skip = 2; // stations between lights
	Ogre::Vector3 station_pos[stations];
	for(size_t x=0; x<s_to_top; ++x)
	{
		float theta = ((float)x/stations_per_revolution)*2.0f*Ogre::Math::PI;
		station_pos[x] = base+Ogre::Vector3(Ogre::Math::Sin(theta)*r, ascend*x, Ogre::Math::Cos(theta)*r);
	}
	for(size_t x=s_to_top; x<stations; ++x)
	{
		float theta = ((float)x/stations_per_revolution)*2.0f*Ogre::Math::PI;
		station_pos[x] = base+Ogre::Vector3(Ogre::Math::Sin(theta)*r, h-ascend*(x-s_to_top), Ogre::Math::Cos(theta)*r);
	}
	// Create a track for the light swarm
	Ogre::Animation *animation = scene->createAnimation("CircleAnimation", stations * seconds_per_station);
	// Spline it for nice curves
	animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);

	DynamicLight *light = NULL;
	for (int x = 1; x <= 10; x++)
	{
		for (int y = 1; y <= 10; y++)
		{
			light = dss->CreatePointLight(base + Ogre::Vector3(x, 0, y));
			//light->GetOgreLight()->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
			light->GetOgreLight()->setDiffuseColour(GetRandomColor());
			light->SetSpecularColor(GetRandomColor());
			light->SetAttenuation(6, 1, 0, 7);

			float range = 400;
			//light->SetAttenuation(range, 1.f, 4.5f / range, 75.f / (range * range));

			Ogre::SceneNode *node = scene->getRootSceneNode()->createChildSceneNode();
			node->attachObject(light->GetOgreLight());

			// Create a track to animate the camera's node
			Ogre::NodeAnimationTrack* track = animation->createNodeTrack(x * ((x * (y + x)) + y), node);
			for(size_t s=0; s<=stations; ++s)
			{
				// Setup keyframes
				Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(s*seconds_per_station); // A start position
				key->setTranslate(station_pos[(x*skip+s)%stations]);
			}
		}
	}

	light = dss->CreateSpotLight(Ogre::Vector3(15, 3, 0));
	//light->GetOgreLight()->setDirection(Ogre::Vector3(0, 0.5f, -0.5f));
	light->GetOgreLight()->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
	light->GetOgreLight()->setDiffuseColour(GetRandomColor());
	light->GetOgreLight()->setCastShadows(true);
	light->SetSpecularColor(GetRandomColor());
    light->SetAttenuation(100, 1, 0.1, 0);

	light = dss->CreateSpotLight(Ogre::Vector3(23, 0, 0));
	light->GetOgreLight()->setDirection(Ogre::Vector3(-1, 0.5f, 1));
	light->GetOgreLight()->setDiffuseColour(GetRandomColor());
	light->GetOgreLight()->setCastShadows(true);
	light->SetSpecularColor(GetRandomColor());
    light->SetAttenuation(100, 1, 0.1, 0);

	for (int i = 0; i < 8; i++)
	{
		Ogre::SceneNode *knotNode = scene->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity *knot = scene->createEntity("knot.mesh");
		unsigned short src, dest;
		if (!knot->getMesh()->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
			knot->getMesh()->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		knot->setMaterialName("RockWall");
		knotNode->attachObject(knot);
		knotNode->setPosition(Ogre::Vector3(25.5, 2, -5.5) + Ogre::Vector3(i * -3, 0, 0));
		knotNode->setScale(Ogre::Vector3(0.01f));
		light = dss->CreatePointLight(Ogre::Vector3(25.5, 4, -5.5) + Ogre::Vector3(i * -3, 0, 0));
		light->GetOgreLight()->setDiffuseColour(GetRandomColor());
		light->SetSpecularColor(GetRandomColor());
		light->SetAttenuation(5, 0.4, 0, 0.4);
	}

	Ogre::AnimationState *state = scene->createAnimationState("CircleAnimation");
	state->setEnabled(true);
	state->setLoop(true);

	float lastTime = graphic->getTimer()->getMilliseconds();
	float time;
	while (!window->isClosed())
	{
		Ogre::WindowEventUtilities::messagePump();

		keyboard->capture();
		mouse->capture();

		if (keyboard->isKeyDown(OIS::KC_ESCAPE))
			break;

		static const float TranslateStep = 0.2f;
		static const float RotateStep = 0.09f;
		Ogre::Vector3 translate = Ogre::Vector3::ZERO;

		if (keyboard->isKeyDown(OIS::KC_W))
			translate.z -= TranslateStep;
		if (keyboard->isKeyDown(OIS::KC_S))
			translate.z += TranslateStep;
		if (keyboard->isKeyDown(OIS::KC_A))
			translate.x -= TranslateStep;
		if (keyboard->isKeyDown(OIS::KC_D))
			translate.x += TranslateStep;
		
		if (keyboard->isKeyDown(OIS::KC_O))
			dss->SetEnabled(true);
		if (keyboard->isKeyDown(OIS::KC_P))
			dss->SetEnabled(false);
		
		if (keyboard->isKeyDown(OIS::KC_U))
			dss->SetEnabledSSAO(true);
		if (keyboard->isKeyDown(OIS::KC_I))
			dss->SetEnabledSSAO(false);

		//float range = light->GetOgreLight()->getAttenuationRange();
		//if (keyboard->isKeyDown(OIS::KC_1))
		//	range += 1;
		//if (keyboard->isKeyDown(OIS::KC_2))
		//	range -= 1;
		//float constant = light->GetOgreLight()->getAttenuationConstant();
		//if (keyboard->isKeyDown(OIS::KC_3))
		//	constant += 0.01f;
		//if (keyboard->isKeyDown(OIS::KC_4))
		//	constant -= 0.01f;
		//float linear = light->GetOgreLight()->getAttenuationLinear();
		//if (keyboard->isKeyDown(OIS::KC_5))
		//	linear += 0.01f;
		//if (keyboard->isKeyDown(OIS::KC_6))
		//	linear -= 0.01f;
		//float quadric = light->GetOgreLight()->getAttenuationQuadric();
		//if (keyboard->isKeyDown(OIS::KC_7))
		//	quadric += 0.01f;
		//if (keyboard->isKeyDown(OIS::KC_8))
		//	quadric -= 0.01f;
		//light->SetAttenuation(range, constant, linear, quadric);
		
		if (keyboard->isKeyDown(OIS::KC_F1))
			dss->SetRenderMode(DeferredShadingSystem::RM_DEPTH_SPECULAR);
		if (keyboard->isKeyDown(OIS::KC_F2))
			dss->SetRenderMode(DeferredShadingSystem::RM_NORMALS);
		if (keyboard->isKeyDown(OIS::KC_F3))
			dss->SetRenderMode(DeferredShadingSystem::RM_COLOR);
		if (keyboard->isKeyDown(OIS::KC_F4))
			dss->SetRenderMode(DeferredShadingSystem::RM_LIT);

		camera->moveRelative(translate);

		camera->pitch(Ogre::Degree(-mouse->getMouseState().Y.rel * RotateStep));
		camera->yaw(Ogre::Degree(-mouse->getMouseState().X.rel * RotateStep));

		SetWindowText((HWND)handle, Ogre::StringConverter::toString(window->getStatistics().lastFPS).c_str());
		//SetWindowText((HWND)handle, (Ogre::String(
		//	"range : ") + Ogre::StringConverter::toString(range) + 
		//	" const : " + Ogre::StringConverter::toString(constant) + 
		//	" linear : " + Ogre::StringConverter::toString(linear) + 
		//	" quadric : " + Ogre::StringConverter::toString(quadric)).c_str());

		time = graphic->getTimer()->getMilliseconds();
		state->addTime((time - lastTime) / 1000.f);
		lastTime = time;

		graphic->renderOneFrame();
	}
}