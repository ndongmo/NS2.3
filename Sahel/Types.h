/*!
* \file Types.h
* \brief Simple file containing different types definition.
* \author Ndongmo Silatsa Fabrice
* \date 22-12-2017
* \version 2.0
*/

#pragma once
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include <string>
#include <tuple>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <CoreTypes.h>

#define MAX_HP 2000
#define MAX_STAMINA 160
#define DATE_FORMAT "%d.%m.%Y %H:%M:%S"

/*!
* \brief Different types of entities.
* DYNAMIC : movable entity, able to move itself thanks to an AI.
* KINEMATIC : movable entity, but not able to move itself. Additionally,
*	it is a static body in the dynamic body physical set. its particularity is that
*	as static body it does not trigger collision. However, when move by a dynamic 
*	body, it becomes a dynamic body able to collide.
* STATIC : not movable entity, so it doesn't  need to be updated.
*/
enum EntityState { DYNAMIC, STATIC, KINEMATIC };

/*!
 * \brief Agent class types.
 */
enum AgentClass
{
	CLASS_SOLDIER,
	CLASS_WARRIOR,
	CLASS_GANGSTER,
};

/*!
 * \brief Action dialog types.
 */
enum ActionDialogType
{
	ACTION_CONFIRM,
	ACTION_LOOT,
	ACTION_REPAIR,
	ACTION_CODE,
	ACTION_DIALOG,
	ACTION_NOTIFICATION,
	ACTION_CLOSE
};

/*!
 * \brief Weather elements.
 */
enum WeatherType
{
	WEATHER_NONE,
	WEATHER_FOG,
	WEATHER_SMOKE,
};

/*!
 * \brief Weather elements.
 */
enum GameState
{
	GAME_STARTING = 0,
	GAME_RUNNING = 1,
	GAME_PAUSING = 2,
	GAME_CHECKPOINT = 3,
	GAME_FORGING = 4,
	GAME_LOADING = 5,
	GAME_TRANSITION = 6,
	GAME_CLOSING = 7,
	GAME_BOOTING = 8,
	GAMESTATE_COUNT = 9
};

/*!
 * \brief Player events.
 */
enum PlayerEvent 
{
	ACTION,
	USE_ITEM,
	ROLL_RUN,
	TWO_HAND,
	SWITCH_LEFTW,
	SWITCH_RIGHTW,
	SWITCH_OBJECT,
	SWITCH_AMMO,
	R1ATTACK,
	R2ATTACK,
	L1ATTACK,
	L2ATTACK,
	LOCK_TARGET,
	GO_FORWARD,
	GO_BACKWARD,
	GO_LEFT,
	GO_RIGHT,
	MENU,
	MAP,
};

/*!
 * \brief Angle orientation.
 */
enum Orientation 
{ 
	NORMAL = 360, 
	UP = 90, 
	DOWN = 270, 
	LEFT = 180, 
	RIGHT = 0 
};

/*!
 * \brief Different type of Sensor. Hide the below layer by applying a black texture.
 * SENSOR_AREA: simple sensor to detect player entering and inform the listeners
 * SENSOR_HIDE1: apply SENSOR_AREA behaviors and additionally will hide the below layer
 *   until the player enters in the area once.
 * SENSOR_HIDE2: apply SENSOR_HIDE1 behaviors except that it will always hide the zone
 *   when the user is not inside.
 * SENSOR_VIEW: view sensor area
 * SENSOR_SOUND: sound sensor area
 * SENSOR_LEVEL: area to move to another level
 */
enum SensorType
{
	SENSOR_AREA = 0,
	SENSOR_HIDE1 = 1,
	SENSOR_HIDE2 = 2,
	SENSOR_VIEW = 3,
	SENSOR_SOUND = 4,
	SENSOR_LEVEL = 5,
};

/*!
 * \brief Different type of Key.
 */
enum KeyType
{
	CRANK = 0,
	CARD = 1,
	KEY = 2,
	PLATE = 3,
	TOOLS = 4,
	CODE = 5
};

/*!
 * \brief Different type of Decor.
 */
enum DecorType
{
	DECOR_NORMAL = 0,
	DECOR_MOVABLE = 1,
	DECOR_INVISIBLE = 2,
	DECOR_BREAKABLE = 3,
	DECOR_ROOF = 4,
	DECOR_CHECKPOINT = 5,
	DECOR_FORGE = 6,
};

/*!
 * \brief Different types of Item.
 */
enum ShopType
{
	SHOP_NONE = 0x0,	// 0000
	SHOP_BUY = 0x2,		// 0010
	SHOP_SELL = 0x4,	// 0100
	SHOP_FORGE = 0x8,	// 1000
	SHOP_ALL = 0xf,		// 1111
};

/*!
 * \brief Different type of Trap.
 */
enum TrapType
{
	CRUSHING = 0,
	PIERCING = 1,
	SWALLOWING = 2
};

/*!
 * \brief Different type of Quest.
 */
enum QuestType
{
	DESTROY = 0,	// destroy one or many entities
	PROTECT = 1,	// protect one or many entities
	CARRY = 2,		// carry one object to an npc
	DISCOVER = 3	// discover a place or an object
};

/*!
 * \brief Different state of Quest.
 */
enum QuestState
{
	IDLE = 0,
	ACTIVE = 1,
	SUCCEEDED = 2,
	FAILED = 3
};

/*!
 * \brief Different type of AIs.
 */
enum AIType
{
	PLAYER = 0,
	NPC = 1,
	ENEMY = 2
};

/*!
 * \brief Different state of AIs.
 */
enum AIState
{
	AI_IDLE = 0,
	AI_WANDERING = 1,
	AI_CHASING = 2,
	AI_ATTAKING = 3
};

/*!
 * \brief Different type of materials.
 */
enum MaterialType
{
	NONE = 0,
	ORGANIC = 1,
	WOOD = 2,
	GLASS = 3,
	PAPER = 4,
	TEXTILES = 5,
	LIQUID = 6,
	STONE = 7,
	VEGETAL = 8,
	METALS = 9,
	PLASTIC = 10,
	GROUND = 11,
	SAND = 12,
	GRAVEL = 13,
};

/*!
 * \brief Different types jewel effect.
 */
enum EffectType
{
	HP = 0,					// increase HP
	STAMINA = 1,			// increase Stamina
	EFFECT_HP = 2,			// cause an effect on HP (increase with time)
	EFFECT_STAMINA = 3,		// cause an effect on Stamina
	POISE = 4,				// increase the poise
	LUCK = 5,				// increase the luck
	SOULS = 6,				// increase the quantity of absorbed soul
	POISON_RES = 7,			// increase poison resistance
	FIRE_RES = 8,			// increase fire resistance
	MAGIC_RES = 9,			// increase magic resistance
	LIGHTNING_RES = 10,		// increase lightning resistance
	PHYSIC_RES = 11,		// increase physic resistance
	NB_EFFECTS = 12			// Effects count
};
/*!
 * \brief Different types of throwing object.
 */
enum ThrowingType
{
	ARROW = 0,
	BOLT = 1,
	B_GUN = 2,
	B_RIFFLE = 3,
	B_SHOTGUN = 4,
	SPELL = 5,
	BOMB = 6,
	BATTERY = 7,
};
/*!
 * \brief Different types of Item.
 */
enum ItemType 
{
	ITEM_WEAPON =			0xff00000,			// 1111 1111 0000 0000 0000 0000 0000
	ITEM_WEAPON_STRIKE =	0x8000000,			// 1000 0000 0000 0000 0000 0000 0000 
	ITEM_WEAPON_THROW =		0x4000000,			// 0100 0000 0000 0000 0000 0000 0000
	ITEM_WEAPON_SHIELD =	0x2000000,			// 0010 0000 0000 0000 0000 0000 0000
	ITEM_WEAPON_FIST =		0x1000000,			// 0001 0000 0000 0000 0000 0000 0000
	ITEM_WEAPON_LIGHT =		0x0800000,			// 0000 1000 0000 0000 0000 0000 0000
	ITEM_CONSUMABLE =		0x00ff000,			// 0000 0000 1111 1111 0000 0000 0000
	ITEM_RECOVERY =			0x0080000,
	ITEM_SOUL =				0x0040000,
	ITEM_THROWING =			0x0020000,
	ITEM_BOOSTWEAPON =		0x0010000,
	ITEM_ARMOR =			0x0000800,
	ITEM_KEY =				0x0000400,
	ITEM_JEWEL =			0x0000200,
	ITEM_AMMO =				0x0000100,
	ITEM_STONE =			0x0000080,
	ITEM_DOCUMENT =			0x0000040,
	ITEM_ITEM =				0xfffffff
};

/*!
 * \brief Different types of Bonus.
 */
enum BonusType
{
	F = 0,
	E = 1,
	D = 2,
	C = 3,
	B = 4,
	A = 5,
	S = 6
};

/*!
* \brief Different types of upgrade.
*/
enum UpgradeType
{
	UPGRADE_NORMAL = 0,			// For normal stuffs (weapon, armor, ...)
	UPGRADE_UNIQUE = 1,			// For unique stuffs (weapon, armor, ...)
	UPGRADE_SPECIAL = 2,		
};
/*!
* \brief Different types of entities.
*/
enum EntityType {
	ENTITY_WEAPON_THROW = 0,
	ENTITY_WEAPON_STRIKE = 1,
	ENTITY_WEAPON_SHIELD = 2,
	ENTITY_WEAPON_FIST = 3,
	ENTITY_WEAPON_LIGHT = 4,
	ENTITY_AGENT = 5,
	ENTITY_DECOR = 6,
	ENTITY_COFFER = 7,
	ENTITY_PICKABLE = 8,
	ENTITY_TRAP = 9,
	ENTITY_LIFT = 10,
	ENTITY_DOOR = 11,
	ENTITY_ACTIVATOR = 12,
	ENTITY_LAMP = 13,
	ENTITY_SENSOR = 14,
	ENTITY_PLAYER = 15,
	ENTITY_ENEMY = 16,
	ENTITY_NPC = 17,
	ENTITY_COUNT = 18
};

/*!
 * \brief Entity identifier.
 */
enum EntityID
{
	ID_AGENT =			0xff000000,			// 1111 1111 0000 0000 0000 0000 0000 0000
	ID_PLAYER =			0xc0000000,			// 1100 0000 0000 0000 0000 0000 0000 0000
	ID_NPC =			0x30000000,			// 0011 0000 0000 0000 0000 0000 0000 0000
	ID_ENEMY =			0x0C000000,			// 0000 1100 0000 0000 0000 0000 0000 0000
	ID_WEAPON =			0x00ff0000,			// 0000 0000 1111 1111 0000 0000 0000 0000
	ID_WFIST =			0x00800000,			// 0000 0000 1000 0000 0000 0000 0000 0000
	ID_WSTRIKE =		0x00400000,			// 0000 0000 0100 0000 0000 0000 0000 0000
	ID_WTHROW =			0x00200000,			// 0000 0000 0010 0000 0000 0000 0000 0000
	ID_WSHIELD =		0x00100000,			// 0000 0000 0001 0000 0000 0000 0000 0000
	ID_WLIGHT =			0x00080000,			// 0000 0000 0000 1000 0000 0000 0000 0000
	ID_OBJECT =			0x0000fff0,			// 0000 0000 0000 0000 1111 1111 1111 0000
	ID_DECOR =			0x00008000,			// 0000 0000 0000 0000 1000 0000 0000 0000
	ID_PICKABLE =		0x00004000,			// 0000 0000 0000 0000 0100 0000 0000 0000
	ID_COFFER =			0x00002000,			// 0000 0000 0000 0000 0010 0000 0000 0000
	ID_TRAP =			0x00001000,			// 0000 0000 0000 0000 0001 0000 0000 0000
	ID_LIFT =			0x00000800,			// 0000 0000 0000 0000 0000 1000 0000 0000
	ID_DOOR =			0x00000400,			// 0000 0000 0000 0000 0000 0100 0000 0000
	ID_ACTIVATOR =		0x00000200,			// 0000 0000 0000 0000 0000 0010 0000 0000
	ID_LAMP =			0x00000100,			// 0000 0000 0000 0000 0000 0001 0000 0000
	ID_DIVERS =			0x0000000f,			// 0000 0000 0000 0000 0000 0000 0000 1111
	ID_SENSOR =			0x00000008,			// 0000 0000 0000 0000 0000 0000 0000 1000
	ID_THROW =			0x00000004,			// 0000 0000 0000 0000 0000 0000 0000 0100
	ID_ENTITY =			0xffffffff,
	ID_NONE =			0x00000000
};

enum Shape2DType
{
	CIRCLE = 0, 
	BOX = 1, 
	ROTATED_BOX = 2, 
	POLYGON = 3, 
	TRIANGLE = 4,  
	SHAPE_COUNT = 5
};

enum DepthType
{
	DEPTH_GROUND = 0,
	DEPTH_GROUND_UP = 2,
	DEPTH_FLOOR = 4,
	DEPTH_FLOOR_UP = 6,
	DEPTH_CARPET = 8,
	DEPTH_CARPET_UP = 10,
	DEPTH_LEFT_FOOT = 12,
	DEPTH_LEFT_FOOT_UP = 14,
	DEPTH_RIGHT_FOOT = 16,
	DEPTH_RIGHT_FOOT_UP = 18,
	DEPTH_LEFT_LEG = 20,
	DEPTH_LEFT_LEG_UP = 22,
	DEPTH_RIGHT_LEG = 24,
	DEPTH_RIGHT_LEG_UP = 26,
	DEPTH_BODY_HIP = 27,
	DEPTH_BODY = 28,
	DEPTH_BODY_UP = 30,
	DEPTH_LEFT_HAND = 32,
	DEPTH_LEFT_HAND_UP = 34,
	DEPTH_RIGHT_HAND = 36,
	DEPTH_RIGHT_HAND_UP = 38,
	DEPTH_LEFT_FOREARM = 40,
	DEPTH_LEFT_FOREARM_UP = 42,
	DEPTH_RIGHT_FOREARM = 44,
	DEPTH_RIGHT_FOREARM_UP = 46,
	DEPTH_LEFT_ARM = 48,
	DEPTH_LEFT_ARM_UP = 50,
	DEPTH_RIGHT_ARM = 52,
	DEPTH_RIGHT_ARM_UP = 54,
	DEPTH_BODY_NECK = 55,
	DEPTH_HEAD = 56,
	DEPTH_HEAD_UP = 58,
	DEPTH_TREE = 60,
	DEPTH_TREE_UP = 62,
	DEPTH_CLOUD = 64,
	DEPTH_COUNT = 33
};

/*!
* \brief Human part enum.
*/
enum HumanPart
{
	HP_HIP = 0,
	HP_BODY = 1,
	HP_HEAD = 2,
	HP_LEFT_ARM = 3,
	HP_RIGHT_ARM = 4,
	HP_LEFT_FOREARM = 5,
	HP_RIGHT_FOREARM = 6,
	HP_LEFT_HAND = 7,
	HP_RIGHT_HAND = 8,
	HP_LEFT_LEG = 9,
	HP_RIGHT_LEG = 10,
	HP_LEFT_FOOT = 11,
	HP_RIGHT_FOOT = 12,
	HP_COUNT = 13
};

enum HumanJoint
{
	HJ_NECK = 0,
	HJ_LEFT_SHOULDER = 1,
	HJ_RIGHT_SOULDER = 2,
	HJ_LEFT_ELBOW = 3,
	HJ_RIGHT_ELBOW = 4,
	HJ_LEFT_WRIST = 5,
	HJ_RIGHT_WRIST = 6,
	HJ_LEFT_HIP = 7,
	HJ_RIGHT_HIP = 8,
	HJ_LEFT_ANKLE = 9,
	HJ_RIGHT_ANKLE = 10,
	HJ_BODY = 11,
	HJ_COUNT = 12
};

enum PartType 
{
	PART_BODY = 0,
	PART_HEAD = 1,
	PART_LEFT_ARM = 2,
	PART_RIGHT_ARM = 3,
	PART_LEFT_FOREARM = 4,
	PART_RIGHT_FOREARM = 5,
	PART_LEFT_HAND = 6,
	PART_RIGHT_HAND = 7,
	PART_LEFT_LEG = 8,
	PART_RIGHT_LEG = 9,
	PART_LEFT_FOOT = 10,
	PART_RIGHT_FOOT = 11,
	PART_TILE = 12,
	PART_GUN = 13,
	PART_SWORD = 14,
	PART_FIST = 15,
	PART_AMMO = 16,
	PART_FLASHLIGHT = 17,
	PART_WALL = 18,
	PART_COFFER_BASE = 19,
	PART_COFFER_TOP = 20,
	PART_DOOR = 21,
	PART_DECOR = 22,
	PART_EMPTY = 23,
	PART_LIFT = 24,
	PART_ACTIVATOR_BASE = 25,
	PART_ACTIVATOR_TOP = 26,
	PART_TRAP = 27,
	PART_SENSOR = 28,
	PART_LAMP = 29,
	PART_PROJECTILE = 30,
	PART_SHIELD = 31,
	PART_HIP = 32,
	PART_COUNT = 33
};

enum GameType
{
	TYPE_TRANSITION = 0,
	TYPE_BODYPART = 1,
	TYPE_BODY = 2,
	TYPE_PART_ANIM = 3,
	TYPE_BODY_ANIM = 4,
	TYPE_ARMOR = 5,
	TYPE_CRAFTSTONE = 6,
	TYPE_KEY = 7,
	TYPE_JEWEL = 8,
	TYPE_AMMO = 9,
	TYPE_DOCUMENT = 10,
	TYPE_RECOVERY_ITEM = 11,
	TYPE_SOUL_ITEM = 12,
	TYPE_WEAPON_ITEM = 13,
	TYPE_THROWING_ITEM = 14,
	TYPE_WEAPON_SHIELD = 15,
	TYPE_WEAPON_STRIKE = 16,
	TYPE_WEAPON_THROW = 17,
	TYPE_WEAPON_FIST = 18,
	TYPE_WEAPON_LIGHT = 19,
	TYPE_AGENT = 20,
	TYPE_PLAYER = 21,
	TYPE_DECOR = 22,
	TYPE_COFFER = 23,
	TYPE_PICKABLE = 24,
	TYPE_TRAP = 25,
	TYPE_LIFT = 26,
	TYPE_DOOR = 27,
	TYPE_ACTIVATOR = 28,
	TYPE_LAMP = 29,
	TYPE_TILE = 30,
	TYPE_QUEST = 31,
	TYPE_DIALOG = 32,
	TYPE_PLAYER_AI = 33,
	TYPE_NPC_AI = 34,
	TYPE_ENEMY_AI = 35,
	TYPE_EMITTER = 36,
	TYPE_LIGHT = 37,
	TYPE_SENSOR = 38,
	TYPE_SPRITE = 39,
	TYPE_GRAPH_EDGE = 40,
	TYPE_GRAPH_NODE = 41,
	TYPE_COUNT = 42
};

enum BodyType
{
	BODY_HUMAN = 0,
	BODY_TILE = 1,
	BODY_GUN = 2,
	BODY_SWORD = 3,
	BODY_FIST = 4,
	BODY_WALL = 5,
	BODY_AMMO = 6,
	BODY_FLASHLIGHT = 7,
	BODY_DECOR = 8,
	BODY_COFFER = 9,
	BODY_DOOR = 10,
	BODY_EMPTY = 11,
	BODY_LIFT = 12,
	BODY_ACTIVATOR = 13,
	BODY_TRAP = 14,
	BODY_SENSOR = 15,
	BODY_LAMP = 16,
	BODY_PROJECTILE = 17,
	BODY_SHIELD = 18,
	BODY_COUNT = 19
};

enum class DialogTask
{
	QUIT,
	CHANGE,
	GAMETYPE,
	GAMEOBJECT,
	LOADING,
	CREATION,
	DELETION,
	NAMING,
	EDIT,
	LOADBODY,
	LOADPART,
	LOADANIM,
	LOADENTITY,
	NOTHING
};

/*!
* \brief Damage data structure.
*/
struct DamageData
{
	float physic = 0.0f;
	float magic = 0.0f;
	float fire = 0.0f;
	float lightning = 0.0f;

	DamageData() { }
	DamageData(float ph, float ma, float fi, float li) : 
		physic(ph), magic(ma), fire(fi), lightning(li) { }

	void reset()
	{
		physic = 0.0f;
		magic = 0.0;
		fire = 0.0f;
		lightning = 0.0f;
	}
	void operator += (const DamageData& data)
	{
		physic += data.physic; 
		magic += data.magic;
		fire += data.fire;
		lightning += data.lightning;
	}
	void upgrade(float upgradeRate) {
		physic += physic * upgradeRate;
		magic += magic * upgradeRate;
		fire += fire * upgradeRate;
		lightning += lightning * upgradeRate;
	}
};

/*!
* \brief Effect data structure.
*/
struct EffectData
{
	float bleed = 0.0f;
	float poison = 0.0f;
	float curse = 0.0f;

	EffectData() { }
	EffectData(float bl, float po, float cu) :
		bleed(bl), poison(po), curse(cu) { }
	void reset()
	{
		bleed = 0.0f;
		poison = 0.0;
		curse = 0.0f;
	}
	void operator += (const EffectData& data)
	{
		bleed += data.bleed;
		poison += data.poison;
		curse += data.curse;
	}
	void upgrade(float upgradeRate) {
		bleed += bleed * upgradeRate;
		poison += poison * upgradeRate;
		curse += curse * upgradeRate;
	}	
};

/*!
* \brief Required stat data structure.
*/
struct StatData
{
	unsigned int strength = 0;
	unsigned int dexterity = 0;
	unsigned int intelligence = 0;
	unsigned int faith = 0;

	void reset()
	{
		strength = 0;
		dexterity = 0;
		intelligence = 0;
		faith = 0;
	}
};

/*!
* \brief Required stat data structure.
*/
struct BonusData
{
	BonusType strength = BonusType::F;
	BonusType dexterity = BonusType::F;
	BonusType intelligence = BonusType::F;
	BonusType faith = BonusType::F;

	void reset()
	{
		strength = BonusType::F;
		dexterity = BonusType::F;
		intelligence = BonusType::F;
		faith = BonusType::F;
	}
	void upgrade() {
		if (strength != BonusType::S) strength = static_cast<BonusType>(static_cast<int>(strength) + 1);
		if (dexterity != BonusType::S) dexterity = static_cast<BonusType>(static_cast<int>(dexterity) + 1);
		if (intelligence != BonusType::S) intelligence = static_cast<BonusType>(static_cast<int>(intelligence) + 1);
		if (faith != BonusType::S) faith = static_cast<BonusType>(static_cast<int>(faith) + 1);
	}
};

struct Effect
{
	float m_time = 0.0f;
	float m_value = 0.0f;
	float m_elapsedTime = 0.0f;
	EffectType m_type = EffectType::NB_EFFECTS;

	void reset()
	{
		m_time = 0.0f;
		m_value = 0.0f;
		m_elapsedTime = 0.0f;
		m_type = EffectType::NB_EFFECTS;
	}
};

/*!
* \brief Level description data structure.
*/
struct LevelData
{
	std::string name;
	std::string description;
	std::string filename;
};

/*!
* \brief Transition data structure.
*/
struct TransitionData
{
	GameState state =  GameState::GAME_STARTING;
	std::string path;
};

/*!
* \brief Save data structure.
*/
struct Save
{
	unsigned int time = 0;		//play time
	std::time_t creation = 0;	//creation datetime
	std::time_t update = 0;		//last update datetime
	std::string dir;			//save dir
	std::string level;			//current level file name
	std::string player;			//player's file
	std::string player_name;	//player's name
	std::string level_name;		//level's name

	void setCreation(const char* time_str) {
		creation = getTimeByString(time_str);
	}
	void setUpdate(const char* time_str) {
		update = getTimeByString(time_str);
	}
	std::string getCreation() const {
		return timeToString(creation);
	}
	std::string getUpdate() const {
		return timeToString(update);
	}
	std::string getDirPath() const {
		std::string date = getCreation();
		size_t i = 0;
		while ((i = date.find_first_of('.', i)) != std::string::npos) {
			date = date.replace(i, 1, "_");
		}
		i = 0;
		while ((i = date.find_first_of(' ', i)) != std::string::npos) {
			date = date.replace(i, 1, "_");
		}
		i = 0;
		while ((i = date.find_first_of(':', i)) != std::string::npos) {
			date = date.replace(i, 1, "_");
		}
		return date;
	}
	std::string timeToString(const std::time_t& t) const {
		char buffer[80];
		strftime(buffer, 80, DATE_FORMAT, localtime(&t));
		return std::string(buffer);
	}
	std::time_t getTimeByString(const char* time_str) {
		struct std::tm tm = { 0 };
		std::istringstream ss(time_str);
		ss >> std::get_time(&tm, DATE_FORMAT);
		return mktime(&tm);
	}
	std::string getSpendTime() const {
		int hour = time / 3600;
		int min = (time % 3600) / 60;
		int sec = ((time % 3600) % 60) / 60;
		return std::to_string(hour) + "h " + std::to_string(min) + " min " + std::to_string(sec) + "s";
	}
	void save() {
		update = std::time(0);
		time = (int)std::difftime(update, creation); //in seconds
	}
};

/*!
* \brief Save history data structure.
*/
struct SaveData
{
	std::vector<Save> saves;
	
	Save getLastSave() {
		Save data;
		if (!saves.empty()) {
			data = saves.back();
		}
		return data;
	}
	void sortByTime() {
		std::sort(saves.begin(), saves.end(), [](const Save& t1, const Save& t2) {
			return std::difftime(t2.update, t1.update) >= 0.0; //decreasing order
		});
	}
	void addSave(const Save& save) {
		saves.push_back(save); //add in last position
	}
};

/*!
* \brief Game data structure.
*/
struct GameData
{
	std::vector<SaveData> saves;
	std::unordered_map<std::string, LevelData> levels;
	std::unordered_map<unsigned char, TransitionData> transitions;

	LevelData getLevelData(const std::string& filename) {
		LevelData data;
		if (levels.find(filename) != levels.end()) {
			data = levels[filename];
		}
		return data;
	}
	TransitionData getTransitionData(GameState gs) {
		TransitionData data;
		if (transitions.find(gs) != transitions.end()) {
			data = transitions[gs];
		}
		return data;
	}
	SaveData getLastSave() {
		SaveData data;
		if (!saves.empty()) {
			data = saves[0];
		}
		return data;
	}
};

class TypeHelper
{
public:
	static void init();
	static void clear();
	static void initFolders();
	static bool saveGameData();
	static bool saveSaveData();

	static int getOppositePart(BodyType type, int partType);
	static int getOppositeJoint(BodyType type, int jointType);
	static int getEntityIndex(GameType type);
	static GameType getGameType(ItemType type);
	static GameType getGameType(EntityType type);
	static GameType getGameType(EntityID id);
	static EntityID getEntityID(EntityType type);
	static EntityType getEntityType(EntityID id);
	static unsigned int getPartIndex(int partType);
	static unsigned int getPartCount(BodyType type);
	static unsigned int getJointCount(BodyType type);

	static PartType getPartOf(BodyType type, int part);

	static std::string getEntity(EntityID id);
	static std::string getKey(KeyType type);
	static std::string getSensor(SensorType type);
	static std::string getDecor(DecorType type);
	static std::string getTrap(TrapType type);
	static std::string getBonusType(BonusType type);
	static std::string getAIType(AIType type);
	static std::string getItemType(ItemType type);
	static std::string getUpgradeType(UpgradeType type);
	static std::string getMaterialType(MaterialType type);
	static std::string getDepthType(DepthType type);
	static std::string getShapeType(Shape2DType type);
	static std::string getBodyType(BodyType type);
	static std::string getPartType(PartType type);
	static std::string getPartType(BodyType type, int part);
	static std::string getJointType(BodyType type, int joint);
	static std::string getEntityType(EntityType type);
	static std::string getGameType(GameType type);
	static std::string getAgentClass(AgentClass aClass);

	static void createKeyTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createSensorTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createDecorTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createTrapTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createBonusTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createAITypes(std::unordered_map<unsigned char, std::string> &m);
	static void createHumanPartTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createHumanJointTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createBodyTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createPartTypes(std::unordered_map<unsigned char, std::pair<unsigned char, std::string>> &m);
	static void createEntityTypes(std::unordered_map<int, std::string> &m);
	static void createGameTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createDepthTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createShapeTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createItemTypes(std::unordered_map<int, std::string> &m);
	static void createUpgradeTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createMaterialTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createEffectTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createThrowingTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createQuestTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createQuestStates(std::unordered_map<unsigned char, std::string> &m);
	static void createWeatherTypes(std::unordered_map<unsigned char, std::string> &m);
	static void createGameStates(std::unordered_map<unsigned char, std::string> &m);
	static void createFontAlignment(std::unordered_map<unsigned char, std::string> &m);
	static void createAgentClasses(std::unordered_map<unsigned char, std::string>& m);
	static void createShopSkills(std::unordered_map<unsigned char, std::string>& m);

	static std::unordered_map<unsigned char, std::string> shopSkills;
	static std::unordered_map<unsigned char, std::string> agentClasses;
	static std::unordered_map<unsigned char, std::string> keyTypes;
	static std::unordered_map<unsigned char, std::string> sensorTypes;
	static std::unordered_map<unsigned char, std::string> decorTypes;
	static std::unordered_map<unsigned char, std::string> trapTypes;
	static std::unordered_map<unsigned char, std::string> bonusTypes;
	static std::unordered_map<unsigned char, std::string> aiTypes;
	static std::unordered_map<unsigned char, std::string> gameTypes;
	static std::unordered_map<unsigned char, std::string> materialTypes;
	static std::unordered_map<unsigned char, std::string> shapeTypes;
	static std::unordered_map<unsigned char, std::string> depthTypes;
	static std::unordered_map<unsigned char, std::string> upgradeTypes;
	static std::unordered_map<unsigned char, std::string> effectTypes;
	static std::unordered_map<unsigned char, std::string> throwingTypes;
	static std::unordered_map<unsigned char, std::string> questTypes;
	static std::unordered_map<unsigned char, std::string> questStates;
	static std::unordered_map<unsigned char, std::string> weatherTypes;
	static std::unordered_map<unsigned char, std::string> gameStates;
	static std::unordered_map<unsigned char, std::string> fontAlignments;
	static std::unordered_map<int, std::string> entityTypes;
	static std::unordered_map<int, std::string> itemTypes;

	static std::unordered_map<unsigned char, std::string> bodyTypes;
	static std::unordered_map<unsigned char, std::string> human_partTypes;
	static std::unordered_map<unsigned char, std::string> human_jointTypes;
	static std::unordered_map<unsigned char, std::pair<unsigned char, std::string>> partTypes;

	static GameData gameData;
	static SaveData saveData;

	static const NS2::ColorRGBA8 white;
	static const NS2::ColorRGBA8 black;
	static const NS2::ColorRGBA8 red;
	static const NS2::ColorRGBA8 green;
	static const NS2::ColorRGBA8 blue;
	static const NS2::ColorRGBA8 yellow;

private:
	static bool m_init;
};
