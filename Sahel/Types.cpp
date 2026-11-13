#include "Types.h"
#include "ObjectManager.h"
#include "PathHelper.h"

bool TypeHelper::m_init = false; 
std::unordered_map<unsigned char, std::string> TypeHelper::questStates;
std::unordered_map<unsigned char, std::string> TypeHelper::questTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::keyTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::sensorTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::decorTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::trapTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::bonusTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::aiTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::effectTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::throwingTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::gameTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::shapeTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::depthTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::depthTypes2;
std::unordered_map<unsigned char, std::string> TypeHelper::upgradeTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::materialTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::weatherTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::gameStates;
std::unordered_map<unsigned char, std::string> TypeHelper::fontAlignments;
std::unordered_map<unsigned char, std::string> TypeHelper::agentClasses;
std::unordered_map<unsigned char, std::string> TypeHelper::shopSkills;
std::unordered_map<int, std::string> TypeHelper::entityTypes;
std::unordered_map<int, std::string> TypeHelper::itemTypes;

std::unordered_map<unsigned char, std::string> TypeHelper::partTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::bodyTypes;
std::unordered_map<unsigned char, std::vector<BodyPart>> TypeHelper::bodyParts;
std::unordered_map<unsigned char, std::vector<BodyJoint>> TypeHelper::bodyJoints;

GameData TypeHelper::gameData;
SaveData TypeHelper::saveData;

const NS2::ColorRGBA8 TypeHelper::white(255, 255, 255, 255);
const NS2::ColorRGBA8 TypeHelper::black(0, 0, 0, 255);
const NS2::ColorRGBA8 TypeHelper::red(255, 0, 0, 255);
const NS2::ColorRGBA8 TypeHelper::green(0, 255, 0, 255);
const NS2::ColorRGBA8 TypeHelper::blue(0, 0, 255, 255);
const NS2::ColorRGBA8 TypeHelper::yellow(255, 255, 0, 255);

void TypeHelper::init()
{
	if (!m_init) {
		initFolders();

		createShopSkills();
		createAgentClasses();
		createFontAlignment();
		createGameStates();
		createWeatherTypes();
		createQuestStates();
		createQuestTypes();
		createKeyTypes();
		createSensorTypes();
		createDecorTypes();
		createTrapTypes();
		createBonusTypes();
		createAITypes();
		createMaterialTypes();
		createEffectTypes();
		createThrowingTypes();
		createUpgradeTypes();
		createItemTypes();
		createShapeTypes();
		createDepthTypes();
		createDepthTypes2();
		createBodyTypes();
		createGameTypes();
		createEntityTypes();
		createPartTypes();
		createBodyParts();
		createBodyJoints();

		//ObjectManager<GameData>::load<GameDataHandler>(gameData, PathHelper::getUtilsPath("LEVELS"));
		//ObjectManager<SaveData>::load<SaveDataHandler>(saveData, SAVES_FILE);
		saveData.sortByTime();

		m_init = true;
	}
}

void TypeHelper::clear()
{
	m_init = false;
	gameTypes.clear();
	shopSkills.clear();
	agentClasses.clear();
	fontAlignments.clear();
	gameStates.clear();
	weatherTypes.clear();
	questStates.clear();
	questTypes.clear();
	keyTypes.clear();
	sensorTypes.clear();
	decorTypes.clear();
	trapTypes.clear();
	bonusTypes.clear();
	aiTypes.clear();
	materialTypes.clear();
	effectTypes.clear();
	throwingTypes.clear();
	upgradeTypes.clear();
	itemTypes.clear();
	shapeTypes.clear();
	depthTypes.clear();
	bodyTypes.clear();
	partTypes.clear();
	entityTypes.clear();
	bodyParts.clear();
	bodyJoints.clear();
	depthTypes2.clear();
}

void TypeHelper::initFolders()
{
	PathHelper::createDir(SAVEPATH);
}

bool TypeHelper::saveGameData()
{
	//return ObjectManager<GameData>::save(gameData, PathHelper::getUtilsPath("LEVELS"));
	return true;
}

bool TypeHelper::saveSaveData()
{
	//return ObjectManager<SaveData>::save(saveData, SAVES_FILE);
	return true;
}

const BodyPart& TypeHelper::getBodyPart(int partType)
{
	for (const auto& BodyPart : bodyParts) {
		for (const auto& p : BodyPart.second) {
			if (p.partType == partType) {
				return p;
			}
		}
	}
	throw std::out_of_range("Part not found");
}
const BodyJoint* TypeHelper::getParent(BodyType type, int childPart)
{
	for (const auto& j : bodyJoints[type]) {
		if (j.childIndex == childPart) {
			return &j;
		}
	}
	return nullptr;
}
std::vector<const BodyJoint*> TypeHelper::getChildren(BodyType type, int parentPart)
{
	std::vector <const BodyJoint*> children;
	for (const auto& j : bodyJoints[type]) {
		if (j.parentIndex == parentPart) {
			children.push_back(&j);
		}
	}
	return children;
}
int TypeHelper::getEntityIndex(GameType type)
{
	switch (type)
	{
	case GameType::TYPE_SENSOR:
		return EntityType::ENTITY_SENSOR;
	case GameType::TYPE_TRAP:
		return EntityType::ENTITY_TRAP;
	case GameType::TYPE_LIFT:
		return EntityType::ENTITY_LIFT;
	case GameType::TYPE_DOOR:
		return EntityType::ENTITY_DOOR;
	case GameType::TYPE_ACTIVATOR:
		return EntityType::ENTITY_ACTIVATOR;
	case GameType::TYPE_LAMP:
		return EntityType::ENTITY_LAMP;
	case GameType::TYPE_PICKABLE:
		return EntityType::ENTITY_PICKABLE;
	case GameType::TYPE_COFFER:
		return EntityType::ENTITY_COFFER;
	case GameType::TYPE_DECOR:
		return EntityType::ENTITY_DECOR;
	case GameType::TYPE_AGENT:
		return EntityType::ENTITY_AGENT;
	case GameType::TYPE_PLAYER:
		return EntityType::ENTITY_PLAYER;
	case GameType::TYPE_WEAPON_LIGHT:
		return EntityType::ENTITY_WEAPON_LIGHT;
	case GameType::TYPE_WEAPON_FIST:
		return EntityType::ENTITY_WEAPON_FIST;
	case GameType::TYPE_WEAPON_SHIELD:
		return EntityType::ENTITY_WEAPON_SHIELD;
	case GameType::TYPE_WEAPON_STRIKE:
		return EntityType::ENTITY_WEAPON_STRIKE;
	case GameType::TYPE_WEAPON_THROW:
		return EntityType::ENTITY_WEAPON_THROW;
	}
	return -1;
}
GameType TypeHelper::getGameType(ItemType type)
{
	switch (type)
	{
	case ItemType::ITEM_WEAPON_STRIKE:
		return GameType::TYPE_WEAPON_STRIKE;
	case ItemType::ITEM_WEAPON_THROW:
		return GameType::TYPE_WEAPON_THROW;
	case ItemType::ITEM_WEAPON_SHIELD:
		return GameType::TYPE_WEAPON_SHIELD;
	case ItemType::ITEM_WEAPON_FIST:
		return GameType::TYPE_WEAPON_FIST;
	case ItemType::ITEM_WEAPON_LIGHT:
		return GameType::TYPE_WEAPON_LIGHT;
	case ItemType::ITEM_ARMOR:
		return GameType::TYPE_ARMOR;
	case ItemType::ITEM_KEY:
		return GameType::TYPE_KEY;
	case ItemType::ITEM_JEWEL:
		return GameType::TYPE_JEWEL;
	case ItemType::ITEM_AMMO:
		return GameType::TYPE_AMMO;
	case ItemType::ITEM_STONE:
		return GameType::TYPE_CRAFTSTONE;
	case ItemType::ITEM_RECOVERY:
		return GameType::TYPE_RECOVERY_ITEM;
	case ItemType::ITEM_SOUL:
		return GameType::TYPE_SOUL_ITEM;
	case ItemType::ITEM_THROWING:
		return GameType::TYPE_THROWING_ITEM;
	case ItemType::ITEM_BOOSTWEAPON:
		return GameType::TYPE_WEAPON_ITEM;
	case ItemType::ITEM_DOCUMENT:
		return GameType::TYPE_DOCUMENT;
	}
	return GameType::TYPE_AMMO;
}
GameType TypeHelper::getGameType(EntityType type)
{
	switch (type)
	{
	case EntityType::ENTITY_SENSOR:
		return GameType::TYPE_SENSOR;
	case EntityType::ENTITY_TRAP:
		return GameType::TYPE_TRAP;
	case EntityType::ENTITY_LIFT:
		return GameType::TYPE_LIFT;
	case EntityType::ENTITY_DOOR:
		return GameType::TYPE_DOOR;
	case EntityType::ENTITY_ACTIVATOR:
		return GameType::TYPE_ACTIVATOR;
	case EntityType::ENTITY_LAMP:
		return GameType::TYPE_LAMP;
	case EntityType::ENTITY_DECOR:
		return GameType::TYPE_DECOR;
	case EntityType::ENTITY_COFFER:
		return GameType::TYPE_COFFER;
	case EntityType::ENTITY_PICKABLE:
		return GameType::TYPE_PICKABLE;
	case EntityType::ENTITY_PLAYER:
		return GameType::TYPE_PLAYER;
	case EntityType::ENTITY_AGENT:
	case EntityType::ENTITY_ENEMY:
	case EntityType::ENTITY_NPC:
		return GameType::TYPE_AGENT;
	case EntityType::ENTITY_WEAPON_FIST:
		return GameType::TYPE_WEAPON_FIST;
	case EntityType::ENTITY_WEAPON_LIGHT:
		return GameType::TYPE_WEAPON_LIGHT;
	case EntityType::ENTITY_WEAPON_SHIELD:
		return GameType::TYPE_WEAPON_SHIELD;
	case EntityType::ENTITY_WEAPON_STRIKE:
		return GameType::TYPE_WEAPON_STRIKE;
	case EntityType::ENTITY_WEAPON_THROW:
		return GameType::TYPE_WEAPON_THROW;
	}
	return GameType::TYPE_DECOR;
}
GameType TypeHelper::getGameType(EntityID id)
{
	return getGameType(getEntityType(id));
}
EntityID TypeHelper::getEntityID(EntityType type)
{
	switch (type)
	{
	case EntityType::ENTITY_PLAYER:
		return EntityID::ID_PLAYER;
	case EntityType::ENTITY_ENEMY:
		return EntityID::ID_ENEMY;
	case EntityType::ENTITY_NPC:
		return EntityID::ID_NPC;
	case EntityType::ENTITY_SENSOR:
		return EntityID::ID_SENSOR;
	case EntityType::ENTITY_TRAP:
		return EntityID::ID_TRAP;
	case EntityType::ENTITY_LIFT:
		return EntityID::ID_LIFT;
	case EntityType::ENTITY_DOOR:
		return EntityID::ID_DOOR;
	case EntityType::ENTITY_ACTIVATOR:
		return EntityID::ID_ACTIVATOR;
	case EntityType::ENTITY_LAMP:
		return EntityID::ID_LAMP;
	case EntityType::ENTITY_DECOR:
		return EntityID::ID_DECOR;
	case EntityType::ENTITY_COFFER:
		return EntityID::ID_COFFER;
	case EntityType::ENTITY_PICKABLE:
		return EntityID::ID_PICKABLE	;
	case EntityType::ENTITY_AGENT:
		return EntityID::ID_AGENT;
	case EntityType::ENTITY_WEAPON_LIGHT:
		return EntityID::ID_WLIGHT;
	case EntityType::ENTITY_WEAPON_FIST:
		return EntityID::ID_WFIST;
	case EntityType::ENTITY_WEAPON_SHIELD:
		return EntityID::ID_WSHIELD;
	case EntityType::ENTITY_WEAPON_STRIKE:
		return EntityID::ID_WSTRIKE;
	case EntityType::ENTITY_WEAPON_THROW:
		return EntityID::ID_WTHROW;
	}
	return EntityID::ID_DECOR;
}
EntityType TypeHelper::getEntityType(EntityID id)
{
	switch (id)
	{
	case EntityID::ID_PLAYER:
		return EntityType::ENTITY_PLAYER;
	case EntityID::ID_ENEMY:
		return EntityType::ENTITY_ENEMY;
	case EntityID::ID_NPC:
		return EntityType::ENTITY_NPC;
	case EntityID::ID_SENSOR:
		return EntityType::ENTITY_SENSOR;
	case EntityID::ID_TRAP:
		return EntityType::ENTITY_TRAP;
	case EntityID::ID_LIFT:
		return EntityType::ENTITY_LIFT;
	case EntityID::ID_DOOR:
		return EntityType::ENTITY_DOOR;
	case EntityID::ID_ACTIVATOR:
		return EntityType::ENTITY_ACTIVATOR;
	case EntityID::ID_LAMP:
		return EntityType::ENTITY_LAMP;
	case EntityID::ID_DECOR:
		return EntityType::ENTITY_DECOR;
	case EntityID::ID_COFFER:
		return EntityType::ENTITY_COFFER;
	case EntityID::ID_PICKABLE:
		return EntityType::ENTITY_PICKABLE;
	case EntityID::ID_AGENT:
		return EntityType::ENTITY_AGENT;
	case EntityID::ID_WLIGHT:
		return EntityType::ENTITY_WEAPON_LIGHT;
	case EntityID::ID_WFIST:
		return EntityType::ENTITY_WEAPON_FIST;
	case EntityID::ID_WSHIELD:
		return EntityType::ENTITY_WEAPON_SHIELD;
	case EntityID::ID_WSTRIKE:
		return EntityType::ENTITY_WEAPON_STRIKE;
	case EntityID::ID_WTHROW:
		return EntityType::ENTITY_WEAPON_THROW;
	}
	return EntityType::ENTITY_DECOR;
}
std::string TypeHelper::getBodyType(BodyType type)
{
	return bodyTypes.at(type);
}

std::string TypeHelper::getDepthType(DepthType type)
{
	return depthTypes.at(type);
}

std::string TypeHelper::getShapeType(Shape2DType type)
{
	return shapeTypes.at(type);
}

std::string TypeHelper::getGameType(GameType type)
{
	return gameTypes.at(type);
}
std::string TypeHelper::getAgentClass(AgentClass aClass)
{
	return agentClasses.at(aClass);
}

DepthType TypeHelper::getDepthType2(int type2)
{
	for (const auto& [typeKey, typeName] : depthTypes) {
		if (typeName == depthTypes2[type2]) {
			return static_cast<DepthType>(typeKey);
		}
	}
	throw std::out_of_range("depth not found");
}

std::string TypeHelper::getEntityType(EntityType type)
{
	return entityTypes.at(type);
}

std::string TypeHelper::getEntity(EntityID id)
{
	return getGameType(getGameType(id));
}

std::string TypeHelper::getKey(KeyType type)
{
	return keyTypes.at(type);
}

std::string TypeHelper::getSensor(SensorType type)
{
	return sensorTypes.at(type);
}

std::string TypeHelper::getDecor(DecorType type)
{
	return decorTypes.at(type);
}

std::string TypeHelper::getTrap(TrapType type)
{
	return trapTypes.at(type);
}

std::string TypeHelper::getBonusType(BonusType type)
{
	return bonusTypes.at(type);
}

std::string TypeHelper::getAIType(AIType type)
{
	return aiTypes.at(type);
}

std::string TypeHelper::getItemType(ItemType type)
{
	return itemTypes.at(type);
}

std::string TypeHelper::getUpgradeType(UpgradeType type)
{
	return upgradeTypes.at(type);
}

std::string TypeHelper::getMaterialType(MaterialType type)
{
	return materialTypes.at(type);
}

void TypeHelper::createKeyTypes()
{
	keyTypes[KeyType::CRANK] = "CRANK";
	keyTypes[KeyType::CARD] = "CARD";
	keyTypes[KeyType::KEY] = "KEY";
	keyTypes[KeyType::PLATE] = "PLATE";
	keyTypes[KeyType::TOOLS] = "TOOLS";
	keyTypes[KeyType::CODE] = "CODE";
}

void TypeHelper::createSensorTypes()
{
	sensorTypes[SensorType::SENSOR_AREA] = "AREA";
	sensorTypes[SensorType::SENSOR_HIDE1] = "HIDE1";
	sensorTypes[SensorType::SENSOR_HIDE2] = "HIDE2";
	sensorTypes[SensorType::SENSOR_VIEW] = "VIEW";
	sensorTypes[SensorType::SENSOR_SOUND] = "SOUND";
	sensorTypes[SensorType::SENSOR_LEVEL] = "LEVEL";
}

void TypeHelper::createDecorTypes()
{
	decorTypes[DecorType::DECOR_NORMAL] = "NORMAL";
	decorTypes[DecorType::DECOR_MOVABLE] = "MOVABLE";
	decorTypes[DecorType::DECOR_INVISIBLE] = "INVISIBLE";
	decorTypes[DecorType::DECOR_BREAKABLE] = "BREAKABLE";
	decorTypes[DecorType::DECOR_ROOF] = "ROOF";
	decorTypes[DecorType::DECOR_CHECKPOINT] = "CHECKPOINT";
	decorTypes[DecorType::DECOR_FORGE] = "FORGE";
}

void TypeHelper::createTrapTypes()
{
	trapTypes[TrapType::CRUSHING] = "CRUSHING";
	trapTypes[TrapType::PIERCING] = "PIERCING";
	trapTypes[TrapType::SWALLOWING] = "SWALLOWING";
}

void TypeHelper::createBonusTypes()
{
	bonusTypes[BonusType::A] = "A";
	bonusTypes[BonusType::B] = "B";
	bonusTypes[BonusType::C] = "C";
	bonusTypes[BonusType::D] = "D";
	bonusTypes[BonusType::E] = "E";
	bonusTypes[BonusType::F] = "F";
	bonusTypes[BonusType::S] = "S";
}

void TypeHelper::createAITypes()
{
	aiTypes[AIType::PLAYER] = "PLAYER";
	aiTypes[AIType::NPC] = "NPC";
	aiTypes[AIType::ENEMY] = "ENEMY";
}

void TypeHelper::createEntityTypes()
{
	entityTypes[EntityType::ENTITY_WEAPON_SHIELD] = "SHIELD WEAPON";
	entityTypes[EntityType::ENTITY_WEAPON_STRIKE] = "STRIKE WEAPON";
	entityTypes[EntityType::ENTITY_WEAPON_THROW] = "THROW WEAPON";
	entityTypes[EntityType::ENTITY_WEAPON_FIST] = "FIST WEAPON";
	entityTypes[EntityType::ENTITY_WEAPON_LIGHT] = "LIGHT WEAPON";
	entityTypes[EntityType::ENTITY_AGENT] = "AGENT";
	entityTypes[EntityType::ENTITY_PLAYER] = "PLAYER";
	entityTypes[EntityType::ENTITY_ENEMY] = "ENEMY";
	entityTypes[EntityType::ENTITY_NPC] = "NPC";
	entityTypes[EntityType::ENTITY_DECOR] = "DECOR";
	entityTypes[EntityType::ENTITY_COFFER] = "COFFER";
	entityTypes[EntityType::ENTITY_PICKABLE] = "PICKABLE";
	entityTypes[EntityType::ENTITY_TRAP] = "TRAP";
	entityTypes[EntityType::ENTITY_LIFT] = "LIFT";
	entityTypes[EntityType::ENTITY_DOOR] = "DOOR";
	entityTypes[EntityType::ENTITY_ACTIVATOR] = "ACTIVATOR";
	entityTypes[EntityType::ENTITY_LAMP] = "LAMP";
	entityTypes[EntityType::ENTITY_SENSOR] = "SENSOR";
}

void TypeHelper::createGameTypes()
{
	gameTypes[GameType::TYPE_PART] = "PART";
	gameTypes[GameType::TYPE_BODY] = "BODY";
	gameTypes[GameType::TYPE_PART_ANIM] = "ANIM_PART";
	gameTypes[GameType::TYPE_BODY_ANIM] = "ANIM_BODY";
	gameTypes[GameType::TYPE_TRANSITION] = "TRANSITION";
	gameTypes[GameType::TYPE_ARMOR] = "ARMOR";
	gameTypes[GameType::TYPE_CRAFTSTONE] = "CRAFTSTONE";
	gameTypes[GameType::TYPE_KEY] = "KEY";
	gameTypes[GameType::TYPE_ACTIVATOR] = "ACTIVATOR";
	gameTypes[GameType::TYPE_JEWEL] = "JEWEL";
	gameTypes[GameType::TYPE_AMMO] = "AMMO";
	gameTypes[GameType::TYPE_RECOVERY_ITEM] = "ITEM_RECOVERY";
	gameTypes[GameType::TYPE_SOUL_ITEM] = "ITEM_SOUL";
	gameTypes[GameType::TYPE_WEAPON_ITEM] = "ITEM_WEAPON";
	gameTypes[GameType::TYPE_THROWING_ITEM] = "ITEM_THROWING";
	gameTypes[GameType::TYPE_WEAPON_SHIELD] = "WEAPON_SHIELD";
	gameTypes[GameType::TYPE_WEAPON_STRIKE] = "WEAPON_STRIKE";
	gameTypes[GameType::TYPE_WEAPON_THROW] = "WEAPON_THROW";
	gameTypes[GameType::TYPE_WEAPON_FIST] = "WEAPON_FIST";
	gameTypes[GameType::TYPE_WEAPON_LIGHT] = "WEAPON_LIGHT";
	gameTypes[GameType::TYPE_AGENT] = "AGENT";
	gameTypes[GameType::TYPE_PLAYER] = "PLAYER";
	gameTypes[GameType::TYPE_DECOR] = "DECOR";
	gameTypes[GameType::TYPE_COFFER] = "COFFER";
	gameTypes[GameType::TYPE_PICKABLE] = "PICKABLE";
	gameTypes[GameType::TYPE_ACTIVATOR] = "ACTIVATOR";
	gameTypes[GameType::TYPE_LIFT] = "LIFT";
	gameTypes[GameType::TYPE_LAMP] = "LAMP";
	gameTypes[GameType::TYPE_TRAP] = "TRAP";
	gameTypes[GameType::TYPE_DOOR] = "DOOR";
	gameTypes[GameType::TYPE_TILE] = "TILE";
	gameTypes[GameType::TYPE_QUEST] = "QUEST";
	gameTypes[GameType::TYPE_DIALOG] = "DIALOG";
	gameTypes[GameType::TYPE_PLAYER_AI] = "AI_PLAYER";
	gameTypes[GameType::TYPE_NPC_AI] = "AI_NPC";
	gameTypes[GameType::TYPE_ENEMY_AI] = "AI_ENEMY";
	gameTypes[GameType::TYPE_EMITTER] = "EMITTER";
	gameTypes[GameType::TYPE_LIGHT] = "LIGHT";
	gameTypes[GameType::TYPE_DOCUMENT] = "DOCUMENT";
	gameTypes[GameType::TYPE_SENSOR] = "SENSOR";
	gameTypes[GameType::TYPE_SPRITE] = "SPRITE";
	gameTypes[GameType::TYPE_GRAPH_EDGE] = "GRAPH_EDGE";
	gameTypes[GameType::TYPE_GRAPH_NODE] = "GRAPH_NODE";
}

void TypeHelper::createDepthTypes()
{
	depthTypes[DepthType::DEPTH_GROUND] = "DEPTH_GROUND";
	depthTypes[DepthType::DEPTH_GROUND_UP] = "DEPTH_GROUND_UP";
	depthTypes[DepthType::DEPTH_FLOOR] = "DEPTH_FLOOR";
	depthTypes[DepthType::DEPTH_FLOOR_UP] = "DEPTH_FLOOR_UP";
	depthTypes[DepthType::DEPTH_CARPET] = "DEPTH_CARPET";
	depthTypes[DepthType::DEPTH_CARPET_UP] = "DEPTH_CARPET_UP";
	depthTypes[DepthType::DEPTH_LEFT_FOOT] = "DEPTH_LEFT_FOOT";
	depthTypes[DepthType::DEPTH_LEFT_FOOT_UP] = "DEPTH_LEFT_FOOT_UP";
	depthTypes[DepthType::DEPTH_RIGHT_FOOT] = "DEPTH_RIGHT_FOOT";
	depthTypes[DepthType::DEPTH_RIGHT_FOOT_UP] = "DEPTH_RIGHT_FOOT_UP";
	depthTypes[DepthType::DEPTH_LEFT_LEG] = "DEPTH_LEFT_LEG";
	depthTypes[DepthType::DEPTH_LEFT_LEG_UP] = "DEPTH_LEFT_LEG_UP";
	depthTypes[DepthType::DEPTH_RIGHT_LEG] = "DEPTH_RIGHT_LEG";
	depthTypes[DepthType::DEPTH_RIGHT_LEG_UP] = "DEPTH_RIGHT_LEG_UP";
	depthTypes[DepthType::DEPTH_HIP] = "DEPTH_HIP";
	depthTypes[DepthType::DEPTH_TORSO] = "DEPTH_TORSO";
	depthTypes[DepthType::DEPTH_TORSO_UP] = "DEPTH_TORSO_UP";
	depthTypes[DepthType::DEPTH_LEFT_HAND] = "DEPTH_LEFT_HAND";
	depthTypes[DepthType::DEPTH_LEFT_HAND_UP] = "DEPTH_LEFT_HAND_UP";
	depthTypes[DepthType::DEPTH_RIGHT_HAND] = "DEPTH_RIGHT_HAND";
	depthTypes[DepthType::DEPTH_RIGHT_HAND_UP] = "DEPTH_RIGHT_HAND_UP";
	depthTypes[DepthType::DEPTH_LEFT_FOREARM] = "DEPTH_LEFT_FOREARM";
	depthTypes[DepthType::DEPTH_LEFT_FOREARM_UP] = "DEPTH_LEFT_FOREARM_UP";
	depthTypes[DepthType::DEPTH_RIGHT_FOREARM] = "DEPTH_RIGHT_FOREARM";
	depthTypes[DepthType::DEPTH_RIGHT_FOREARM_UP] = "DEPTH_RIGHT_FOREARM_UP";
	depthTypes[DepthType::DEPTH_LEFT_ARM] = "DEPTH_LEFT_ARM";
	depthTypes[DepthType::DEPTH_LEFT_ARM_UP] = "DEPTH_LEFT_ARM_UP";
	depthTypes[DepthType::DEPTH_RIGHT_ARM] = "DEPTH_RIGHT_ARM";
	depthTypes[DepthType::DEPTH_RIGHT_ARM_UP] = "DEPTH_RIGHT_ARM_UP";
	depthTypes[DepthType::DEPTH_BODY_NECK] = "DEPTH_BODY_NECK";
	depthTypes[DepthType::DEPTH_HEAD] = "DEPTH_HEAD";
	depthTypes[DepthType::DEPTH_HEAD_UP] = "DEPTH_HEAD_UP";
	depthTypes[DepthType::DEPTH_TREE] = "DEPTH_TREE";
	depthTypes[DepthType::DEPTH_TREE_UP] = "DEPTH_TREE_UP";
	depthTypes[DepthType::DEPTH_CLOUD] = "DEPTH_CLOUD";
}

void TypeHelper::createDepthTypes2()
{
	depthTypes2[DepthType2::DEPTH2_GROUND] = "DEPTH_GROUND";
	depthTypes2[DepthType2::DEPTH2_GROUND_UP] = "DEPTH_GROUND_UP";
	depthTypes2[DepthType2::DEPTH2_FLOOR] = "DEPTH_FLOOR";
	depthTypes2[DepthType2::DEPTH2_FLOOR_UP] = "DEPTH_FLOOR_UP";
	depthTypes2[DepthType2::DEPTH2_CARPET] = "DEPTH_CARPET";
	depthTypes2[DepthType2::DEPTH2_CARPET_UP] = "DEPTH_CARPET_UP";
	depthTypes2[DepthType2::DEPTH2_LEFT_FOOT] = "DEPTH_LEFT_FOOT";
	depthTypes2[DepthType2::DEPTH2_LEFT_FOOT_UP] = "DEPTH_LEFT_FOOT_UP";
	depthTypes2[DepthType2::DEPTH2_RIGHT_FOOT] = "DEPTH_RIGHT_FOOT";
	depthTypes2[DepthType2::DEPTH2_RIGHT_FOOT_UP] = "DEPTH_RIGHT_FOOT_UP";
	depthTypes2[DepthType2::DEPTH2_LEFT_LEG] = "DEPTH_LEFT_LEG";
	depthTypes2[DepthType2::DEPTH2_LEFT_LEG_UP] = "DEPTH_LEFT_LEG_UP";
	depthTypes2[DepthType2::DEPTH2_RIGHT_LEG] = "DEPTH_RIGHT_LEG";
	depthTypes2[DepthType2::DEPTH2_RIGHT_LEG_UP] = "DEPTH_RIGHT_LEG_UP";
	depthTypes2[DepthType2::DEPTH2_BODY_HIP] = "DEPTH_HIP";
	depthTypes2[DepthType2::DEPTH2_BODY] = "DEPTH_TORSO";
	depthTypes2[DepthType2::DEPTH2_BODY_UP] = "DEPTH_TORSO_UP";
	depthTypes2[DepthType2::DEPTH2_LEFT_HAND] = "DEPTH_LEFT_HAND";
	depthTypes2[DepthType2::DEPTH2_LEFT_HAND_UP] = "DEPTH_LEFT_HAND_UP";
	depthTypes2[DepthType2::DEPTH2_RIGHT_HAND] = "DEPTH_RIGHT_HAND";
	depthTypes2[DepthType2::DEPTH2_RIGHT_HAND_UP] = "DEPTH_RIGHT_HAND_UP";
	depthTypes2[DepthType2::DEPTH2_LEFT_FOREARM] = "DEPTH_LEFT_FOREARM";
	depthTypes2[DepthType2::DEPTH2_LEFT_FOREARM_UP] = "DEPTH_LEFT_FOREARM_UP";
	depthTypes2[DepthType2::DEPTH2_RIGHT_FOREARM] = "DEPTH_RIGHT_FOREARM";
	depthTypes2[DepthType2::DEPTH2_RIGHT_FOREARM_UP] = "DEPTH_RIGHT_FOREARM_UP";
	depthTypes2[DepthType2::DEPTH2_LEFT_ARM] = "DEPTH_LEFT_ARM";
	depthTypes2[DepthType2::DEPTH2_LEFT_ARM_UP] = "DEPTH_LEFT_ARM_UP";
	depthTypes2[DepthType2::DEPTH2_RIGHT_ARM] = "DEPTH_RIGHT_ARM";
	depthTypes2[DepthType2::DEPTH2_RIGHT_ARM_UP] = "DEPTH_RIGHT_ARM_UP";
	depthTypes2[DepthType2::DEPTH2_BODY_NECK] = "DEPTH_BODY_NECK";
	depthTypes2[DepthType2::DEPTH2_HEAD] = "DEPTH_HEAD";
	depthTypes2[DepthType2::DEPTH2_HEAD_UP] = "DEPTH_HEAD_UP";
	depthTypes2[DepthType2::DEPTH2_TREE] = "DEPTH_TREE";
	depthTypes2[DepthType2::DEPTH2_TREE_UP] = "DEPTH_TREE_UP";
	depthTypes2[DepthType2::DEPTH2_CLOUD] = "DEPTH_CLOUD";
}

void TypeHelper::createShapeTypes()
{
	shapeTypes[Shape2DType::CIRCLE] = "CIRCLE";
	shapeTypes[Shape2DType::BOX] = "BOX";
	shapeTypes[Shape2DType::ROTATED_BOX] = "ROTATED BOX";
	shapeTypes[Shape2DType::POLYGON] = "POLYGON";
	shapeTypes[Shape2DType::TRIANGLE] = "TRIANGLE";
}

void TypeHelper::createBodyTypes()
{
	bodyTypes[BodyType::BODY_HUMAN] = "HUMAN";
	bodyTypes[BodyType::BODY_GUN] = "GUN";
	bodyTypes[BodyType::BODY_AMMO] = "AMMO";
	bodyTypes[BodyType::BODY_FLASHLIGHT] = "FLASHLIGHT";
	bodyTypes[BodyType::BODY_SWORD] = "SWORD";
	bodyTypes[BodyType::BODY_FIST] = "FIST";
	bodyTypes[BodyType::BODY_DECOR] = "DECOR";
	bodyTypes[BodyType::BODY_COFFER] = "COFFER";
	bodyTypes[BodyType::BODY_DOOR] = "DOOR";
	bodyTypes[BodyType::BODY_TILE] = "TILE";
	bodyTypes[BodyType::BODY_WALL] = "WALL";
	bodyTypes[BodyType::BODY_EMPTY] = "EMPTY";
	bodyTypes[BodyType::BODY_LIFT] = "LIFT";
	bodyTypes[BodyType::BODY_ACTIVATOR] = "ACTIVATOR";
	bodyTypes[BodyType::BODY_TRAP] = "TRAP";
	bodyTypes[BodyType::BODY_SENSOR] = "SENSOR";
	bodyTypes[BodyType::BODY_LAMP] = "LAMP";
	bodyTypes[BodyType::BODY_PROJECTILE] = "PROJECTILE";
	bodyTypes[BodyType::BODY_SHIELD] = "SHIELD";
}

void TypeHelper::createItemTypes()
{
	itemTypes[ItemType::ITEM_AMMO] = "AMMO";
	itemTypes[ItemType::ITEM_ARMOR] = "ARMOR";
	itemTypes[ItemType::ITEM_RECOVERY] = "RECOVERY_ITEM";
	itemTypes[ItemType::ITEM_BOOSTWEAPON] = "WEAPON_BOOSTER";
	itemTypes[ItemType::ITEM_CONSUMABLE] = "CONSUMABLE";
	itemTypes[ItemType::ITEM_KEY] = "KEY";
	itemTypes[ItemType::ITEM_JEWEL] = "JEWEL";
	itemTypes[ItemType::ITEM_SOUL] = "SOUL";
	itemTypes[ItemType::ITEM_STONE] = "STONE";
	itemTypes[ItemType::ITEM_THROWING] = "THROWING_ITEM";
	itemTypes[ItemType::ITEM_WEAPON_SHIELD] = "SHIELD";
	itemTypes[ItemType::ITEM_WEAPON_STRIKE] = "STRIKE_WEAPON";
	itemTypes[ItemType::ITEM_WEAPON_THROW] = "THROWING_WEAPON";
	itemTypes[ItemType::ITEM_WEAPON_FIST] = "FIST_WEAPON";
	itemTypes[ItemType::ITEM_WEAPON_LIGHT] = "LIGHT_WEAPON";
	itemTypes[ItemType::ITEM_DOCUMENT] = "DOCUMENT";
}

void TypeHelper::createUpgradeTypes()
{
	upgradeTypes[UpgradeType::UPGRADE_NORMAL] = "NORMAL";
	upgradeTypes[UpgradeType::UPGRADE_UNIQUE] = "UNIQUE";
	upgradeTypes[UpgradeType::UPGRADE_SPECIAL] = "SPECIAL";
}

void TypeHelper::createMaterialTypes()
{
	materialTypes[MaterialType::METALS] = "METALS";
	materialTypes[MaterialType::PLASTIC] = "PLASTIC";
	materialTypes[MaterialType::WOOD] = "WOOD";
	materialTypes[MaterialType::GLASS] = "GLASS";
	materialTypes[MaterialType::PAPER] = "PAPER";
	materialTypes[MaterialType::TEXTILES] = "TEXTILES";
	materialTypes[MaterialType::LIQUID] = "LIQUID";
	materialTypes[MaterialType::STONE] = "STONE";
	materialTypes[MaterialType::VEGETAL] = "VEGETAL";
	materialTypes[MaterialType::ORGANIC] = "ORGANIC";
	materialTypes[MaterialType::GROUND] = "GROUND";
	materialTypes[MaterialType::SAND] = "SAND";
	materialTypes[MaterialType::GRAVEL] = "GRAVEL";
}

void TypeHelper::createEffectTypes()
{
	effectTypes[EffectType::HP] = "HP";
	effectTypes[EffectType::STAMINA] = "STAMINA";
	effectTypes[EffectType::EFFECT_HP] = "EFFECT HP";
	effectTypes[EffectType::EFFECT_STAMINA] = "EFFECT STAMINA";
	effectTypes[EffectType::LUCK] = "LUCK";
	effectTypes[EffectType::SOULS] = "SOULS";
	effectTypes[EffectType::POISE] = "POISE";
	effectTypes[EffectType::FIRE_RES] = "FIRE RES";
	effectTypes[EffectType::MAGIC_RES] = "MAGIC RES";
	effectTypes[EffectType::PHYSIC_RES] = "PHYSIC RES";
	effectTypes[EffectType::POISON_RES] = "POISON RES";
	effectTypes[EffectType::LIGHTNING_RES] = "LIGHTNING RES";
}

void TypeHelper::createThrowingTypes()
{
	throwingTypes[ThrowingType::ARROW] = "ARROW";
	throwingTypes[ThrowingType::BOLT] = "BOLT";
	throwingTypes[ThrowingType::B_GUN] = "GUN BULLET";
	throwingTypes[ThrowingType::B_RIFFLE] = "RIFFLE BULLET";
	throwingTypes[ThrowingType::B_SHOTGUN] = "SHOTGUN BULLET";
	throwingTypes[ThrowingType::SPELL] = "SPELL";
	throwingTypes[ThrowingType::BOMB] = "BOMB";
	throwingTypes[ThrowingType::BATTERY] = "BATTERY";
}

void TypeHelper::createQuestTypes()
{
	questTypes[QuestType::DESTROY] = "DESTROY";
	questTypes[QuestType::PROTECT] = "PROTECT";
	questTypes[QuestType::CARRY] = "CARRY";
	questTypes[QuestType::DISCOVER] = "DISCOVER";
}

void TypeHelper::createQuestStates()
{
	questStates[QuestState::IDLE] = "IDLE";
	questStates[QuestState::ACTIVE] = "ACTIVE";
}

void TypeHelper::createWeatherTypes()
{
	weatherTypes[WeatherType::WEATHER_NONE] = "NONE";
	weatherTypes[WeatherType::WEATHER_FOG] = "FOG";
	weatherTypes[WeatherType::WEATHER_SMOKE] = "SMOKE";
}

void TypeHelper::createGameStates()
{
	gameStates[GameState::GAME_BOOTING] = "GAME_BOOTING";
	gameStates[GameState::GAME_STARTING] = "GAME_STARTING";
	gameStates[GameState::GAME_RUNNING] = "GAME_RUNNING";
	gameStates[GameState::GAME_PAUSING] = "GAME_PAUSING";
	gameStates[GameState::GAME_CHECKPOINT] = "GAME_CHECKPOINT";
	gameStates[GameState::GAME_LOADING] = "GAME_LOADING";
	gameStates[GameState::GAME_TRANSITION] = "GAME_TRANSITION";
	gameStates[GameState::GAME_CLOSING] = "GAME_CLOSING";
}

void TypeHelper::createFontAlignment()
{
	fontAlignments[NS2::Justification::LEFT] = "LEFT";
	fontAlignments[NS2::Justification::MIDDLE] = "MIDDLE";
	fontAlignments[NS2::Justification::MIDDLEX] = "MIDDLEX";
	fontAlignments[NS2::Justification::MIDDLEY] = "MIDDLEY";
	fontAlignments[NS2::Justification::RIGHT] = "RIGHT";
	fontAlignments[NS2::Justification::RIGHTX] = "RIGHTX";
	fontAlignments[NS2::Justification::RIGHTY] = "RIGHTY";
}

void TypeHelper::createAgentClasses()
{
	agentClasses[AgentClass::CLASS_SOLDIER] = "SOLDIER";
	agentClasses[AgentClass::CLASS_WARRIOR] = "WARRIOR";
	agentClasses[AgentClass::CLASS_GANGSTER] = "GANGSTER";
}

void TypeHelper::createShopSkills()
{
	shopSkills[ShopType::SHOP_BUY] = "BUY";
	shopSkills[ShopType::SHOP_SELL] = "SELL";
	shopSkills[ShopType::SHOP_FORGE] = "FORGE";
}

void TypeHelper::createPartTypes()
{
	partTypes[PartType::PART_HIP] = "HIP";
	partTypes[PartType::PART_TORSO] = "TORSO";
	partTypes[PartType::PART_HEAD] = "HEAD";
	partTypes[PartType::PART_LEFT_ARM] = "LEFT_ARM";
	partTypes[PartType::PART_RIGHT_ARM] = "RIGHT_ARM";
	partTypes[PartType::PART_LEFT_FOREARM] = "LEFT_FOREARM";
	partTypes[PartType::PART_RIGHT_FOREARM] = "RIGHT_FOREARM";
	partTypes[PartType::PART_LEFT_HAND] = "LEFT_HAND";
	partTypes[PartType::PART_RIGHT_HAND] = "RIGHT_HAND";
	partTypes[PartType::PART_LEFT_LEG] = "LEFT_LEG";
	partTypes[PartType::PART_RIGHT_LEG] = "RIGHT_LEG";
	partTypes[PartType::PART_LEFT_FOOT] = "LEFT_FOOT";
	partTypes[PartType::PART_RIGHT_FOOT] = "RIGHT_FOOT";
	partTypes[PartType::PART_TILE] = "TILE";
	partTypes[PartType::PART_GUN] = "GUN";
	partTypes[PartType::PART_AMMO] = "AMMO";
	partTypes[PartType::PART_FLASHLIGHT] = "FLASHLIGHT";
	partTypes[PartType::PART_SWORD] = "SWORD";
	partTypes[PartType::PART_FIST] = "FIST";
	partTypes[PartType::PART_WALL] = "WALL";
	partTypes[PartType::PART_DOOR] = "DOOR";
	partTypes[PartType::PART_DECOR] = "DECOR";
	partTypes[PartType::PART_EMPTY] = "EMPTY";
	partTypes[PartType::PART_LIFT] = "LIFT";
	partTypes[PartType::PART_COFFER_BASE] = "COFFER_BASE";
	partTypes[PartType::PART_COFFER_TOP] = "COFFER_TOP";
	partTypes[PartType::PART_ACTIVATOR_BASE] = "ACTIVATOR_BASE";
	partTypes[PartType::PART_ACTIVATOR_TOP] = "ACTIVATOR_TOP";
	partTypes[PartType::PART_TRAP] = "TRAP";
	partTypes[PartType::PART_SENSOR] = "SENSOR";
	partTypes[PartType::PART_LAMP] = "LAMP";
	partTypes[PartType::PART_PROJECTILE] = "PROJECTILE";
	partTypes[PartType::PART_SHIELD] = "SHIELD";
}

void TypeHelper::createBodyParts()
{
	bodyParts[BodyType::BODY_HUMAN] = std::vector<BodyPart>{
			{ BodyType::BODY_HUMAN, PartType::PART_HIP, "HIP", 0, -1 },
			{ BodyType::BODY_HUMAN, PartType::PART_HEAD, "HEAD", 1, -1 },
			{ BodyType::BODY_HUMAN, PartType::PART_TORSO, "TORSO", 2, -1 },
			{ BodyType::BODY_HUMAN, PartType::PART_RIGHT_ARM, "RIGHT_ARM", 3, 6 },
			{ BodyType::BODY_HUMAN, PartType::PART_RIGHT_FOREARM, "RIGHT_FOREARM", 4, 7 },
			{ BodyType::BODY_HUMAN, PartType::PART_RIGHT_HAND, "RIGHT_HAND", 5, 8 },
			{ BodyType::BODY_HUMAN, PartType::PART_LEFT_ARM, "LEFT_ARM", 6, 3 },
			{ BodyType::BODY_HUMAN, PartType::PART_LEFT_FOREARM, "LEFT_FOREARM", 7, 4 },
			{ BodyType::BODY_HUMAN, PartType::PART_LEFT_HAND, "LEFT_HAND", 8, 5 },
			{ BodyType::BODY_HUMAN, PartType::PART_RIGHT_LEG, "RIGHT_LEG", 9, 11 },
			{ BodyType::BODY_HUMAN, PartType::PART_RIGHT_FOOT, "RIGHT_FOOT", 10,12 },
			{ BodyType::BODY_HUMAN, PartType::PART_LEFT_LEG, "LEFT_LEG", 11, 9 },
			{ BodyType::BODY_HUMAN, PartType::PART_LEFT_FOOT, "LEFT_FOOT", 12, 10 }
	};
	bodyParts[BodyType::BODY_COFFER] = std::vector<BodyPart>{
		{ BodyType::BODY_COFFER, PartType::PART_COFFER_BASE, "COFFER_BASE", 0, -1 },
		{ BodyType::BODY_COFFER, PartType::PART_COFFER_TOP, "COFFER_TOP", 1, -1 }
	};
	bodyParts[BodyType::BODY_ACTIVATOR] = std::vector<BodyPart>{
		{ BodyType::BODY_ACTIVATOR, PartType::PART_ACTIVATOR_BASE, "ACTIVATOR_BASE", 0, -1 },
		{ BodyType::BODY_ACTIVATOR, PartType::PART_ACTIVATOR_TOP, "ACTIVATOR_TOP", 1, -1 }
	};
	bodyParts[BodyType::BODY_LIFT] = std::vector<BodyPart>{
		{ BodyType::BODY_LIFT, PartType::PART_LIFT, "LIFT", 0, -1 }
	};
	bodyParts[BodyType::BODY_DOOR] = std::vector<BodyPart>{
		{ BodyType::BODY_DOOR, PartType::PART_DOOR, "DOOR", 0, -1 }
	};
	bodyParts[BodyType::BODY_TILE] = std::vector<BodyPart>{
		{ BodyType::BODY_TILE, PartType::PART_TILE, "TILE", 0, -1 }
	};
	bodyParts[BodyType::BODY_WALL] = std::vector<BodyPart>{
		{ BodyType::BODY_WALL, PartType::PART_WALL, "WALL", 0, -1 }
	};
	bodyParts[BodyType::BODY_GUN] = std::vector<BodyPart>{
		{ BodyType::BODY_GUN, PartType::PART_GUN, "GUN", 0, -1 }
	};
	bodyParts[BodyType::BODY_AMMO] = std::vector<BodyPart>{
		{ BodyType::BODY_AMMO, PartType::PART_AMMO, "AMMO", 0, -1 }
	};
	bodyParts[BodyType::BODY_FLASHLIGHT] = std::vector<BodyPart>{
		{ BodyType::BODY_FLASHLIGHT, PartType::PART_FLASHLIGHT, "FLASHLIGHT", 0, -1 }
	};
	bodyParts[BodyType::BODY_SWORD] = std::vector<BodyPart>{
		{ BodyType::BODY_SWORD, PartType::PART_SWORD, "SWORD", 0, -1 }
	};
	bodyParts[BodyType::BODY_FIST] = std::vector<BodyPart>{
		{ BodyType::BODY_FIST, PartType::PART_FIST, "FIST", 0, -1 }
	};
	bodyParts[BodyType::BODY_DECOR] = std::vector<BodyPart>{
		{ BodyType::BODY_DECOR, PartType::PART_DECOR, "DECOR", 0, -1 }
	};
	bodyParts[BodyType::BODY_EMPTY] = std::vector<BodyPart>{
		{ BodyType::BODY_EMPTY, PartType::PART_EMPTY, "EMPTY", 0, -1 }
	};
	bodyParts[BodyType::BODY_TRAP] = std::vector<BodyPart>{
		{ BodyType::BODY_TRAP, PartType::PART_TRAP, "TRAP", 0, -1 }
	};
	bodyParts[BodyType::BODY_SENSOR] = std::vector<BodyPart>{
		{ BodyType::BODY_SENSOR, PartType::PART_SENSOR, "SENSOR", 0, -1 }
	};
	bodyParts[BodyType::BODY_LAMP] = std::vector<BodyPart>{
		{ BodyType::BODY_LAMP, PartType::PART_LAMP, "LAMP", 0, -1 }
	};
	bodyParts[BodyType::BODY_PROJECTILE] = std::vector<BodyPart>{
		{ BodyType::BODY_PROJECTILE, PartType::PART_PROJECTILE, "PROJECTILE", 0, -1 }
	};
	bodyParts[BodyType::BODY_SHIELD] = std::vector<BodyPart>{
		{ BodyType::BODY_SHIELD, PartType::PART_SHIELD, "SHIELD", 0, -1 }
	};
}

void TypeHelper::createBodyJoints()
{
	bodyJoints[BodyType::BODY_HUMAN] = std::vector<BodyJoint>{
		{ BodyType::BODY_HUMAN, "NECK", 0, 2, 1 }, // child: torso -> head
		{ BodyType::BODY_HUMAN, "HIP_TO_TORSO", 1, 0, 2 }, // child: hip -> torso
		{ BodyType::BODY_HUMAN, "HIP_TO_RIGHT_LEG", 2, 0, 9 }, // child: hip -> right leg
		{ BodyType::BODY_HUMAN, "HIP_TO_LEFT_LEG", 3, 0, 11 }, // child: hip -> left leg
		{ BodyType::BODY_HUMAN, "TORSO_TO_RIGHT_ARM", 4, 2, 3 }, // child: torso -> right arm
		{ BodyType::BODY_HUMAN, "TORSO_TO_LEFT_ARM", 5, 2, 6 }, // child: torso -> left arm
		{ BodyType::BODY_HUMAN, "RIGHT_ARM_TO_FOREARM", 6, 3, 4 }, // child: right arm -> right forearm
		{ BodyType::BODY_HUMAN, "LEFT_ARM_TO_FOREARM", 7, 6, 7 }, // child: left arm -> left forearm
		{ BodyType::BODY_HUMAN, "RIGHT_FOREARM_TO_HAND", 8, 4, 5 }, // child: right forearm -> right hand
		{ BodyType::BODY_HUMAN, "LEFT_FOREARM_TO_HAND", 9, 7, 8 }, // child: left forearm -> left hand
		{ BodyType::BODY_HUMAN, "RIGHT_LEG_TO_FOOT", 10, 9, 10 }, // child: right leg -> right foot
		{ BodyType::BODY_HUMAN, "LEFT_LEG_TO_FOOT", 11, 11, 12 } // child: left leg -> left foot
	};
	bodyJoints[BodyType::BODY_COFFER] = std::vector<BodyJoint>{
		{ BodyType::BODY_COFFER, "COFFER", 0, 0, 1 } // child: coffer base -> coffer top
	};
	bodyJoints[BodyType::BODY_ACTIVATOR] = std::vector<BodyJoint>{
		{ BodyType::BODY_ACTIVATOR, "ACTIVATOR", 0, 0, 1 } // child: activator base -> activator top
	};
}
