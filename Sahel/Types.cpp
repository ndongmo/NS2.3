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
std::unordered_map<unsigned char, std::string> TypeHelper::upgradeTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::materialTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::weatherTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::gameStates;
std::unordered_map<unsigned char, std::string> TypeHelper::fontAlignments;
std::unordered_map<unsigned char, std::string> TypeHelper::agentClasses;
std::unordered_map<unsigned char, std::string> TypeHelper::shopSkills;
std::unordered_map<int, std::string> TypeHelper::entityTypes;
std::unordered_map<int, std::string> TypeHelper::itemTypes;

std::unordered_map<unsigned char, std::string> TypeHelper::bodyTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::human_partTypes;
std::unordered_map<unsigned char, std::string> TypeHelper::human_jointTypes;
std::unordered_map<unsigned char, std::pair<unsigned char, std::string>> TypeHelper::partTypes;
std::unordered_map<unsigned char, std::vector<BodyPart>> TypeHelper::bodyParts;

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

		createShopSkills(shopSkills);
		createAgentClasses(agentClasses);
		createFontAlignment(fontAlignments);
		createGameStates(gameStates);
		createWeatherTypes(weatherTypes);
		createQuestStates(questStates);
		createQuestTypes(questTypes);
		createKeyTypes(keyTypes);
		createSensorTypes(sensorTypes);
		createDecorTypes(decorTypes);
		createTrapTypes(trapTypes);
		createBonusTypes(bonusTypes);
		createAITypes(aiTypes);
		createMaterialTypes(materialTypes);
		createEffectTypes(effectTypes);
		createThrowingTypes(throwingTypes);
		createUpgradeTypes(upgradeTypes);
		createItemTypes(itemTypes);
		createShapeTypes(shapeTypes);
		createDepthTypes(depthTypes);
		createBodyTypes(bodyTypes);
		createPartTypes(partTypes);
		createGameTypes(gameTypes);
		createEntityTypes(entityTypes);
		createHumanPartTypes(human_partTypes);
		createHumanJointTypes(human_jointTypes);
		createBodyParts();

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
	human_jointTypes.clear();
	human_partTypes.clear();
	bodyParts.clear();
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

PartType TypeHelper::getPartOf(BodyType type, int part)
{
	std::string partName = getPartType(type, part);

	for (auto& p : partTypes) {
		if (p.second.second == partName)
			return (PartType)p.first;
	}
	return PartType::PART_TILE;
}
const BodyPart& TypeHelper::getBodyPart(BodyType type, int partType)
{
	for (const auto& p : bodyParts[type]) {
		if (p.partType == partType) {
			return p;
		}
	}
	throw std::out_of_range("Part not found");
}
int TypeHelper::getOppositePart(BodyType type, int partType)
{
	if (type == BodyType::BODY_HUMAN) {
		switch (partType)
		{
		case HumanPart::HP_RIGHT_ARM:
			return HumanPart::HP_LEFT_ARM;
		case HumanPart::HP_RIGHT_FOREARM:
			return HumanPart::HP_LEFT_FOREARM;
		case HumanPart::HP_RIGHT_HAND:
			return HumanPart::HP_LEFT_HAND;
		case HumanPart::HP_RIGHT_LEG:
			return HumanPart::HP_LEFT_LEG;
		case HumanPart::HP_RIGHT_FOOT:
			return HumanPart::HP_LEFT_FOOT;
		case HumanPart::HP_LEFT_ARM:
			return HumanPart::HP_RIGHT_ARM;
		case HumanPart::HP_LEFT_FOREARM:
			return HumanPart::HP_RIGHT_FOREARM;
		case HumanPart::HP_LEFT_HAND:
			return HumanPart::HP_RIGHT_HAND;
		case HumanPart::HP_LEFT_LEG:
			return HumanPart::HP_RIGHT_LEG;
		case HumanPart::HP_LEFT_FOOT:
			return HumanPart::HP_RIGHT_FOOT;
		}
	}
	return -1;
}
int TypeHelper::getOppositeJoint(BodyType type, int jointType)
{
	if (type == BodyType::BODY_HUMAN) {
		switch (jointType)
		{
		case HumanJoint::HJ_RIGHT_SOULDER:
			return HumanJoint::HJ_LEFT_SHOULDER;
		case HumanJoint::HJ_RIGHT_ELBOW:
			return HumanJoint::HJ_LEFT_ELBOW;
		case HumanJoint::HJ_RIGHT_WRIST:
			return HumanJoint::HJ_LEFT_WRIST;
		case HumanJoint::HJ_RIGHT_HIP:
			return HumanJoint::HJ_LEFT_HIP;
		case HumanJoint::HJ_RIGHT_ANKLE:
			return HumanJoint::HJ_LEFT_ANKLE;
		case HumanJoint::HJ_LEFT_SHOULDER:
			return HumanJoint::HJ_RIGHT_SOULDER;
		case HumanJoint::HJ_LEFT_ELBOW:
			return HumanJoint::HJ_RIGHT_ELBOW;
		case HumanJoint::HJ_LEFT_WRIST:
			return HumanJoint::HJ_RIGHT_WRIST;
		case HumanJoint::HJ_LEFT_HIP:
			return HumanJoint::HJ_RIGHT_HIP;
		case HumanJoint::HJ_LEFT_ANKLE:
			return HumanJoint::HJ_RIGHT_ANKLE;
		}
	}
	return -1;
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
unsigned int TypeHelper::getPartIndex(int type)
{
	return partTypes.at(type).first;
}
unsigned int TypeHelper::getPartCount(BodyType type)
{
	switch (type)
	{
	case BODY_HUMAN:
		return static_cast<unsigned int>(human_partTypes.size());
	case BODY_COFFER:
	case BODY_ACTIVATOR:
		return 2;
	default:
		return 1;
	}
}
unsigned int TypeHelper::getJointCount(BodyType type)
{
	switch (type)
	{
	case BODY_HUMAN:
		return static_cast<unsigned int>(human_jointTypes.size());
	case BODY_COFFER:
	case BODY_ACTIVATOR:
		return 1;
	default:
		return 0;
	}
}

std::string TypeHelper::getPartType(PartType type)
{
	return partTypes.at(type).second;
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
std::string TypeHelper::getJointType(BodyType type, int joint)
{
	switch (type)
	{
	case BODY_HUMAN:
		return human_jointTypes.at(joint);
	case BODY_COFFER:
	case BODY_ACTIVATOR:
		return "BASE_TOP";
	default:
		return "NOTHING";
	}
}

std::string TypeHelper::getEntityType(EntityType type)
{
	return entityTypes.at(type);
}

std::string TypeHelper::getPartType(BodyType type, int part)
{
	switch (type)
	{
	case BODY_HUMAN:
		return human_partTypes.at(part);
	case BODY_TILE:
		return partTypes.at(PartType::PART_TILE).second;
	case BODY_LIFT:
		return partTypes.at(PartType::PART_LIFT).second;
	case BODY_GUN:
		return partTypes.at(PartType::PART_GUN).second;
	case BODY_SWORD:
		return partTypes.at(PartType::PART_SWORD).second;
	case BODY_FIST:
		return partTypes.at(PartType::PART_FIST).second;
	case BODY_WALL:
		return partTypes.at(PartType::PART_WALL).second;
	case BODY_AMMO:
		return partTypes.at(PartType::PART_AMMO).second;
	case BODY_FLASHLIGHT:
		return partTypes.at(PartType::PART_FLASHLIGHT).second;
	case BODY_DECOR:
		return partTypes.at(PartType::PART_DECOR).second;
	case BODY_COFFER:
		if(part == partTypes.at(PartType::PART_COFFER_BASE).first)
			return  partTypes.at(PartType::PART_COFFER_BASE).second;
		else
			return  partTypes.at(PartType::PART_COFFER_TOP).second;
	case BODY_ACTIVATOR:
		if (part == partTypes.at(PartType::PART_ACTIVATOR_BASE).first)
			return  partTypes.at(PartType::PART_ACTIVATOR_BASE).second;
		else
			return  partTypes.at(PartType::PART_ACTIVATOR_TOP).second;
	case BODY_DOOR:
		return  partTypes.at(PartType::PART_DOOR).second;
	case BODY_EMPTY:
		return  partTypes.at(PartType::PART_EMPTY).second;
	case BODY_TRAP:
		return  partTypes.at(PartType::PART_TRAP).second;
	case BODY_SENSOR:
		return  partTypes.at(PartType::PART_SENSOR).second;
	case BODY_LAMP:
		return  partTypes.at(PartType::PART_LAMP).second;
	case BODY_PROJECTILE:
		return  partTypes.at(PartType::PART_PROJECTILE).second;
	case BODY_SHIELD:
		return  partTypes.at(PartType::PART_SHIELD).second;
	default:
		throw std::exception();
	}
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

void TypeHelper::createKeyTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[KeyType::CRANK] = "CRANK";
	m[KeyType::CARD] = "CARD";
	m[KeyType::KEY] = "KEY";
	m[KeyType::PLATE] = "PLATE";
	m[KeyType::TOOLS] = "TOOLS";
	m[KeyType::CODE] = "CODE";
}

void TypeHelper::createSensorTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[SensorType::SENSOR_AREA] = "AREA";
	m[SensorType::SENSOR_HIDE1] = "HIDE1";
	m[SensorType::SENSOR_HIDE2] = "HIDE2";
	m[SensorType::SENSOR_VIEW] = "VIEW";
	m[SensorType::SENSOR_SOUND] = "SOUND";
	m[SensorType::SENSOR_LEVEL] = "LEVEL";
}

void TypeHelper::createDecorTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[DecorType::DECOR_NORMAL] = "NORMAL";
	m[DecorType::DECOR_MOVABLE] = "MOVABLE";
	m[DecorType::DECOR_INVISIBLE] = "INVISIBLE";
	m[DecorType::DECOR_BREAKABLE] = "BREAKABLE";
	m[DecorType::DECOR_ROOF] = "ROOF";
	m[DecorType::DECOR_CHECKPOINT] = "CHECKPOINT";
	m[DecorType::DECOR_FORGE] = "FORGE";
}

void TypeHelper::createTrapTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[TrapType::CRUSHING] = "CRUSHING";
	m[TrapType::PIERCING] = "PIERCING";
	m[TrapType::SWALLOWING] = "SWALLOWING";
}

void TypeHelper::createBonusTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[BonusType::A] = "A";
	m[BonusType::B] = "B";
	m[BonusType::C] = "C";
	m[BonusType::D] = "D";
	m[BonusType::E] = "E";
	m[BonusType::F] = "F";
	m[BonusType::S] = "S";
}

void TypeHelper::createAITypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[AIType::PLAYER] = "PLAYER";
	m[AIType::NPC] = "NPC";
	m[AIType::ENEMY] = "ENEMY";
}

void TypeHelper::createHumanPartTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[HumanPart::HP_HIP] = "HIP";
	m[HumanPart::HP_BODY] = "BODY";
	m[HumanPart::HP_HEAD] = "HEAD";
	m[HumanPart::HP_LEFT_ARM] = "LEFT_ARM";
	m[HumanPart::HP_RIGHT_ARM] = "RIGHT_ARM";
	m[HumanPart::HP_LEFT_FOREARM] = "LEFT_FOREARM";
	m[HumanPart::HP_RIGHT_FOREARM] = "RIGHT_FOREARM";
	m[HumanPart::HP_LEFT_HAND] = "LEFT_HAND";
	m[HumanPart::HP_RIGHT_HAND] = "RIGHT_HAND";
	m[HumanPart::HP_LEFT_LEG] = "LEFT_LEG";
	m[HumanPart::HP_RIGHT_LEG] = "RIGHT_LEG";
	m[HumanPart::HP_LEFT_FOOT] = "LEFT_FOOT";
	m[HumanPart::HP_RIGHT_FOOT] = "RIGHT_FOOT";
}

void TypeHelper::createHumanJointTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[HumanJoint::HJ_NECK] = "NECK";
	m[HumanJoint::HJ_LEFT_SHOULDER] = "LEFT_SHOULDER";
	m[HumanJoint::HJ_RIGHT_SOULDER] = "RIGHT_SOULDER";
	m[HumanJoint::HJ_LEFT_ELBOW] = "LEFT_ELBOW";
	m[HumanJoint::HJ_RIGHT_ELBOW] = "RIGHT_ELBOW";
	m[HumanJoint::HJ_LEFT_WRIST] = "LEFT_WRIST";
	m[HumanJoint::HJ_RIGHT_WRIST] = "RIGHT_WRIST";
	m[HumanJoint::HJ_LEFT_HIP] = "LEFT_HIP";
	m[HumanJoint::HJ_RIGHT_HIP] = "RIGHT_HIP";
	m[HumanJoint::HJ_LEFT_ANKLE] = "LEFT_ANKLE";
	m[HumanJoint::HJ_RIGHT_ANKLE] = "RIGHT_ANKLE";
	m[HumanJoint::HJ_BODY] = "BODY";
}

void TypeHelper::createEntityTypes(std::unordered_map<int, std::string> &m)
{
	m[EntityType::ENTITY_WEAPON_SHIELD] = "SHIELD WEAPON";
	m[EntityType::ENTITY_WEAPON_STRIKE] = "STRIKE WEAPON";
	m[EntityType::ENTITY_WEAPON_THROW] = "THROW WEAPON";
	m[EntityType::ENTITY_WEAPON_FIST] = "FIST WEAPON";
	m[EntityType::ENTITY_WEAPON_LIGHT] = "LIGHT WEAPON";
	m[EntityType::ENTITY_AGENT] = "AGENT";
	m[EntityType::ENTITY_PLAYER] = "PLAYER";
	m[EntityType::ENTITY_ENEMY] = "ENEMY";
	m[EntityType::ENTITY_NPC] = "NPC";
	m[EntityType::ENTITY_DECOR] = "DECOR";
	m[EntityType::ENTITY_COFFER] = "COFFER";
	m[EntityType::ENTITY_PICKABLE] = "PICKABLE";
	m[EntityType::ENTITY_TRAP] = "TRAP";
	m[EntityType::ENTITY_LIFT] = "LIFT";
	m[EntityType::ENTITY_DOOR] = "DOOR";
	m[EntityType::ENTITY_ACTIVATOR] = "ACTIVATOR";
	m[EntityType::ENTITY_LAMP] = "LAMP";
	m[EntityType::ENTITY_SENSOR] = "SENSOR";
}

void TypeHelper::createGameTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[GameType::TYPE_PART] = "PART";
	m[GameType::TYPE_BODY] = "BODY";
	m[GameType::TYPE_PART_ANIM] = "ANIM_PART";
	m[GameType::TYPE_BODY_ANIM] = "ANIM_BODY";
	m[GameType::TYPE_TRANSITION] = "TRANSITION";
	m[GameType::TYPE_ARMOR] = "ARMOR";
	m[GameType::TYPE_CRAFTSTONE] = "CRAFTSTONE";
	m[GameType::TYPE_KEY] = "KEY";
	m[GameType::TYPE_ACTIVATOR] = "ACTIVATOR";
	m[GameType::TYPE_JEWEL] = "JEWEL";
	m[GameType::TYPE_AMMO] = "AMMO";
	m[GameType::TYPE_RECOVERY_ITEM] = "ITEM_RECOVERY";
	m[GameType::TYPE_SOUL_ITEM] = "ITEM_SOUL";
	m[GameType::TYPE_WEAPON_ITEM] = "ITEM_WEAPON";
	m[GameType::TYPE_THROWING_ITEM] = "ITEM_THROWING";
	m[GameType::TYPE_WEAPON_SHIELD] = "WEAPON_SHIELD";
	m[GameType::TYPE_WEAPON_STRIKE] = "WEAPON_STRIKE";
	m[GameType::TYPE_WEAPON_THROW] = "WEAPON_THROW";
	m[GameType::TYPE_WEAPON_FIST] = "WEAPON_FIST";
	m[GameType::TYPE_WEAPON_LIGHT] = "WEAPON_LIGHT";
	m[GameType::TYPE_AGENT] = "AGENT";
	m[GameType::TYPE_PLAYER] = "PLAYER";
	m[GameType::TYPE_DECOR] = "DECOR";
	m[GameType::TYPE_COFFER] = "COFFER";
	m[GameType::TYPE_PICKABLE] = "PICKABLE";
	m[GameType::TYPE_ACTIVATOR] = "ACTIVATOR";
	m[GameType::TYPE_LIFT] = "LIFT";
	m[GameType::TYPE_LAMP] = "LAMP";
	m[GameType::TYPE_TRAP] = "TRAP";
	m[GameType::TYPE_DOOR] = "DOOR";
	m[GameType::TYPE_TILE] = "TILE";
	m[GameType::TYPE_QUEST] = "QUEST";
	m[GameType::TYPE_DIALOG] = "DIALOG";
	m[GameType::TYPE_PLAYER_AI] = "AI_PLAYER";
	m[GameType::TYPE_NPC_AI] = "AI_NPC";
	m[GameType::TYPE_ENEMY_AI] = "AI_ENEMY";
	m[GameType::TYPE_EMITTER] = "EMITTER";
	m[GameType::TYPE_LIGHT] = "LIGHT";
	m[GameType::TYPE_DOCUMENT] = "DOCUMENT";
	m[GameType::TYPE_SENSOR] = "SENSOR";
	m[GameType::TYPE_SPRITE] = "SPRITE";
	m[GameType::TYPE_GRAPH_EDGE] = "GRAPH_EDGE";
	m[GameType::TYPE_GRAPH_NODE] = "GRAPH_NODE";
}

void TypeHelper::createDepthTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[DepthType::DEPTH_GROUND] = "DEPTH_GROUND";
	m[DepthType::DEPTH_GROUND_UP] = "DEPTH_GROUND_UP";
	m[DepthType::DEPTH_FLOOR] = "DEPTH_FLOOR";
	m[DepthType::DEPTH_FLOOR_UP] = "DEPTH_FLOOR_UP";
	m[DepthType::DEPTH_CARPET] = "DEPTH_CARPET";
	m[DepthType::DEPTH_CARPET_UP] = "DEPTH_CARPET_UP";
	m[DepthType::DEPTH_LEFT_FOOT] = "DEPTH_LEFT_FOOT";
	m[DepthType::DEPTH_LEFT_FOOT_UP] = "DEPTH_LEFT_FOOT_UP";
	m[DepthType::DEPTH_RIGHT_FOOT] = "DEPTH_RIGHT_FOOT";
	m[DepthType::DEPTH_RIGHT_FOOT_UP] = "DEPTH_RIGHT_FOOT_UP";
	m[DepthType::DEPTH_LEFT_LEG] = "DEPTH_LEFT_LEG";
	m[DepthType::DEPTH_LEFT_LEG_UP] = "DEPTH_LEFT_LEG_UP";
	m[DepthType::DEPTH_RIGHT_LEG] = "DEPTH_RIGHT_LEG";
	m[DepthType::DEPTH_RIGHT_LEG_UP] = "DEPTH_RIGHT_LEG_UP";
	m[DepthType::DEPTH_BODY_HIP] = "DEPTH_BODY_HIP";
	m[DepthType::DEPTH_BODY] = "DEPTH_BODY";
	m[DepthType::DEPTH_BODY_UP] = "DEPTH_BODY_UP";
	m[DepthType::DEPTH_LEFT_HAND] = "DEPTH_LEFT_HAND";
	m[DepthType::DEPTH_LEFT_HAND_UP] = "DEPTH_LEFT_HAND_UP";
	m[DepthType::DEPTH_RIGHT_HAND] = "DEPTH_RIGHT_HAND";
	m[DepthType::DEPTH_RIGHT_HAND_UP] = "DEPTH_RIGHT_HAND_UP";
	m[DepthType::DEPTH_LEFT_FOREARM] = "DEPTH_LEFT_FOREARM";
	m[DepthType::DEPTH_LEFT_FOREARM_UP] = "DEPTH_LEFT_FOREARM_UP";
	m[DepthType::DEPTH_RIGHT_FOREARM] = "DEPTH_RIGHT_FOREARM";
	m[DepthType::DEPTH_RIGHT_FOREARM_UP] = "DEPTH_RIGHT_FOREARM_UP";
	m[DepthType::DEPTH_LEFT_ARM] = "DEPTH_LEFT_ARM";
	m[DepthType::DEPTH_LEFT_ARM_UP] = "DEPTH_LEFT_ARM_UP";
	m[DepthType::DEPTH_RIGHT_ARM] = "DEPTH_RIGHT_ARM";
	m[DepthType::DEPTH_RIGHT_ARM_UP] = "DEPTH_RIGHT_ARM_UP";
	m[DepthType::DEPTH_BODY_NECK] = "DEPTH_BODY_NECK";
	m[DepthType::DEPTH_HEAD] = "DEPTH_HEAD";
	m[DepthType::DEPTH_HEAD_UP] = "DEPTH_HEAD_UP";
	m[DepthType::DEPTH_TREE] = "DEPTH_TREE";
	m[DepthType::DEPTH_TREE_UP] = "DEPTH_TREE_UP";
	m[DepthType::DEPTH_CLOUD] = "DEPTH_CLOUD";
}

void TypeHelper::createShapeTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[Shape2DType::CIRCLE] = "CIRCLE";
	m[Shape2DType::BOX] = "BOX";
	m[Shape2DType::ROTATED_BOX] = "ROTATED BOX";
	m[Shape2DType::POLYGON] = "POLYGON";
	m[Shape2DType::TRIANGLE] = "TRIANGLE";
}

void TypeHelper::createBodyTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[BodyType::BODY_HUMAN] = "HUMAN";
	m[BodyType::BODY_GUN] = "GUN";
	m[BodyType::BODY_AMMO] = "AMMO";
	m[BodyType::BODY_FLASHLIGHT] = "FLASHLIGHT";
	m[BodyType::BODY_SWORD] = "SWORD";
	m[BodyType::BODY_FIST] = "FIST";
	m[BodyType::BODY_DECOR] = "DECOR";
	m[BodyType::BODY_COFFER] = "COFFER";
	m[BodyType::BODY_DOOR] = "DOOR";
	m[BodyType::BODY_TILE] = "TILE";
	m[BodyType::BODY_WALL] = "WALL";
	m[BodyType::BODY_EMPTY] = "EMPTY";
	m[BodyType::BODY_LIFT] = "LIFT";
	m[BodyType::BODY_ACTIVATOR] = "ACTIVATOR";
	m[BodyType::BODY_TRAP] = "TRAP";
	m[BodyType::BODY_SENSOR] = "SENSOR";
	m[BodyType::BODY_LAMP] = "LAMP";
	m[BodyType::BODY_PROJECTILE] = "PROJECTILE";
	m[BodyType::BODY_SHIELD] = "SHIELD";
}

void TypeHelper::createPartTypes(std::unordered_map<unsigned char, std::pair<unsigned char, std::string>> &m)
{
	m[PartType::PART_HIP] = { 0, "HIP" };
	m[PartType::PART_TORSO] = {1, "TORSO"};
	m[PartType::PART_HEAD] = { 2, "HEAD" }; 
	m[PartType::PART_LEFT_ARM] = { 3, "LEFT_ARM" };
	m[PartType::PART_RIGHT_ARM] = { 4, "RIGHT_ARM" };
	m[PartType::PART_LEFT_FOREARM] = { 5, "LEFT_FOREARM" }; 
	m[PartType::PART_RIGHT_FOREARM] = { 6, "RIGHT_FOREARM" }; 
	m[PartType::PART_LEFT_HAND] = { 7, "LEFT_HAND" }; 
	m[PartType::PART_RIGHT_HAND] = { 8, "RIGHT_HAND" }; 
	m[PartType::PART_LEFT_LEG] = { 9, "LEFT_LEG" }; 
	m[PartType::PART_RIGHT_LEG] = { 10, "RIGHT_LEG" };
	m[PartType::PART_LEFT_FOOT] = { 11, "LEFT_FOOT" }; 
	m[PartType::PART_RIGHT_FOOT] = { 12, "RIGHT_FOOT" }; 
	m[PartType::PART_TILE] = { 0, "TILE" };
	m[PartType::PART_GUN] = { 0, "GUN" };
	m[PartType::PART_AMMO] = { 0, "AMMO" }; 
	m[PartType::PART_FLASHLIGHT] = { 0, "FLASHLIGHT" }; 
	m[PartType::PART_SWORD] = { 0, "SWORD" };
	m[PartType::PART_FIST] = { 0, "FIST" };
	m[PartType::PART_WALL] = { 0, "WALL" };
	m[PartType::PART_DOOR] = { 0, "DOOR" }; 
	m[PartType::PART_DECOR] = { 0, "DECOR" };
	m[PartType::PART_EMPTY] = { 0, "EMPTY" };
	m[PartType::PART_LIFT] = { 0, "LIFT" };
	m[PartType::PART_COFFER_BASE] = { 0, "COFFER_BASE" };
	m[PartType::PART_COFFER_TOP] = { 1, "COFFER_TOP" };
	m[PartType::PART_ACTIVATOR_BASE] = { 0, "ACTIVATOR_BASE" };
	m[PartType::PART_ACTIVATOR_TOP] = { 1, "ACTIVATOR_TOP" };
	m[PartType::PART_TRAP] = { 0, "TRAP" };
	m[PartType::PART_SENSOR] = { 0, "SENSOR" };
	m[PartType::PART_LAMP] = { 0, "LAMP" };
	m[PartType::PART_PROJECTILE] = { 0, "PROJECTILE" };
	m[PartType::PART_SHIELD] = { 0, "SHIELD" };
}

void TypeHelper::createItemTypes(std::unordered_map<int, std::string> &m)
{
	m[ItemType::ITEM_AMMO] = "AMMO";
	m[ItemType::ITEM_ARMOR] = "ARMOR";
	m[ItemType::ITEM_RECOVERY] = "RECOVERY_ITEM";
	m[ItemType::ITEM_BOOSTWEAPON] = "WEAPON_BOOSTER";
	m[ItemType::ITEM_CONSUMABLE] = "CONSUMABLE";
	m[ItemType::ITEM_KEY] = "KEY";
	m[ItemType::ITEM_JEWEL] = "JEWEL";
	m[ItemType::ITEM_SOUL] = "SOUL";
	m[ItemType::ITEM_STONE] = "STONE";
	m[ItemType::ITEM_THROWING] = "THROWING_ITEM";
	m[ItemType::ITEM_WEAPON_SHIELD] = "SHIELD";
	m[ItemType::ITEM_WEAPON_STRIKE] = "STRIKE_WEAPON";
	m[ItemType::ITEM_WEAPON_THROW] = "THROWING_WEAPON";
	m[ItemType::ITEM_WEAPON_FIST] = "FIST_WEAPON";
	m[ItemType::ITEM_WEAPON_LIGHT] = "LIGHT_WEAPON";
	m[ItemType::ITEM_DOCUMENT] = "DOCUMENT";
}

void TypeHelper::createUpgradeTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[UpgradeType::UPGRADE_NORMAL] = "NORMAL";
	m[UpgradeType::UPGRADE_UNIQUE] = "UNIQUE";
	m[UpgradeType::UPGRADE_SPECIAL] = "SPECIAL";
}

void TypeHelper::createMaterialTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[MaterialType::METALS] = "METALS";
	m[MaterialType::PLASTIC] = "PLASTIC";
	m[MaterialType::WOOD] = "WOOD";
	m[MaterialType::GLASS] = "GLASS";
	m[MaterialType::PAPER] = "PAPER";
	m[MaterialType::TEXTILES] = "TEXTILES";
	m[MaterialType::LIQUID] = "LIQUID";
	m[MaterialType::STONE] = "STONE";
	m[MaterialType::VEGETAL] = "VEGETAL";
	m[MaterialType::ORGANIC] = "ORGANIC";
	m[MaterialType::GROUND] = "GROUND";
	m[MaterialType::SAND] = "SAND";
	m[MaterialType::GRAVEL] = "GRAVEL";
}

void TypeHelper::createEffectTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[EffectType::HP] = "HP";
	m[EffectType::STAMINA] = "STAMINA";
	m[EffectType::EFFECT_HP] = "EFFECT HP";
	m[EffectType::EFFECT_STAMINA] = "EFFECT STAMINA";
	m[EffectType::LUCK] = "LUCK";
	m[EffectType::SOULS] = "SOULS";
	m[EffectType::POISE] = "POISE";
	m[EffectType::FIRE_RES] = "FIRE RES";
	m[EffectType::MAGIC_RES] = "MAGIC RES";
	m[EffectType::PHYSIC_RES] = "PHYSIC RES";
	m[EffectType::POISON_RES] = "POISON RES";
	m[EffectType::LIGHTNING_RES] = "LIGHTNING RES";
}

void TypeHelper::createThrowingTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[ThrowingType::ARROW] = "ARROW";
	m[ThrowingType::BOLT] = "BOLT";
	m[ThrowingType::B_GUN] = "GUN BULLET";
	m[ThrowingType::B_RIFFLE] = "RIFFLE BULLET";
	m[ThrowingType::B_SHOTGUN] = "SHOTGUN BULLET";
	m[ThrowingType::SPELL] = "SPELL";
	m[ThrowingType::BOMB] = "BOMB";
	m[ThrowingType::BATTERY] = "BATTERY";
}

void TypeHelper::createQuestTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[QuestType::DESTROY] = "DESTROY";
	m[QuestType::PROTECT] = "PROTECT";
	m[QuestType::CARRY] = "CARRY";
	m[QuestType::DISCOVER] = "DISCOVER";
}

void TypeHelper::createQuestStates(std::unordered_map<unsigned char, std::string> &m)
{
	m[QuestState::IDLE] = "IDLE";
	m[QuestState::ACTIVE] = "ACTIVE";
}

void TypeHelper::createWeatherTypes(std::unordered_map<unsigned char, std::string> &m)
{
	m[WeatherType::WEATHER_NONE] = "NONE";
	m[WeatherType::WEATHER_FOG] = "FOG";
	m[WeatherType::WEATHER_SMOKE] = "SMOKE";
}

void TypeHelper::createGameStates(std::unordered_map<unsigned char, std::string> &m)
{
	m[GameState::GAME_BOOTING] = "GAME_BOOTING";
	m[GameState::GAME_STARTING] = "GAME_STARTING";
	m[GameState::GAME_RUNNING] = "GAME_RUNNING";
	m[GameState::GAME_PAUSING] = "GAME_PAUSING";
	m[GameState::GAME_CHECKPOINT] = "GAME_CHECKPOINT";
	m[GameState::GAME_LOADING] = "GAME_LOADING";
	m[GameState::GAME_TRANSITION] = "GAME_TRANSITION";
	m[GameState::GAME_CLOSING] = "GAME_CLOSING";
}

void TypeHelper::createFontAlignment(std::unordered_map<unsigned char, std::string> &m)
{
	m[NS2::Justification::LEFT] = "LEFT";
	m[NS2::Justification::MIDDLE] = "MIDDLE";
	m[NS2::Justification::MIDDLEX] = "MIDDLEX";
	m[NS2::Justification::MIDDLEY] = "MIDDLEY";
	m[NS2::Justification::RIGHT] = "RIGHT";
	m[NS2::Justification::RIGHTX] = "RIGHTX";
	m[NS2::Justification::RIGHTY] = "RIGHTY";
}

void TypeHelper::createAgentClasses(std::unordered_map<unsigned char, std::string>& m)
{
	m[AgentClass::CLASS_SOLDIER] = "SOLDIER";
	m[AgentClass::CLASS_WARRIOR] = "WARRIOR";
	m[AgentClass::CLASS_GANGSTER] = "GANGSTER";
}

void TypeHelper::createShopSkills(std::unordered_map<unsigned char, std::string>& m)
{
	m[ShopType::SHOP_BUY] = "BUY";
	m[ShopType::SHOP_SELL] = "SELL";
	m[ShopType::SHOP_FORGE] = "FORGE";
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
