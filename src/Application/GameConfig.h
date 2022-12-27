#pragma once

#include <string>
#include <algorithm>

#include "Library/Config/Config.h"

class Logger;

namespace Application {
    class GameConfig : private Config {
     public:
        GameConfig();
        ~GameConfig();

        void LoadConfiguration();
        void SaveConfiguration();

        class Debug : public ConfigSection {
         public:
            explicit Debug(GameConfig *config) : ConfigSection(config, "debug") {}

            /** Enable all available spells for each character in spellbook bypassing all class restrictions. Currently also all skills will behave like they are on GM level. */
            ConfigValue<bool> AllMagic = ConfigValue<bool>(this, "all_magic", false);
            ConfigValue<bool> InfiniteFood = ConfigValue<bool>(this, "infinite_food", false);
            ConfigValue<bool> InfiniteGold = ConfigValue<bool>(this, "infinite_gold", false);

            /** Debug lightmap and decals outlines. */
            ConfigValue<bool> LightmapDecals = ConfigValue<bool>(this, "lightmap_decals", false);

            /** Draw BLV portal frames. */
            ConfigValue<bool> PortalOutlines = ConfigValue<bool>(this, "portal_outlines", false);
            ConfigValue<bool> Terrain = ConfigValue<bool>(this, "terrain", false);

            /** Bypass only activated by fountains locations block for town portal spell. */
            ConfigValue<bool> TownPortal = ConfigValue<bool>(this, "town_portal", false);

            /** Increase party movement speed by 12x. Most likely you want to use that option with no_damage option enabled as collision physics often will shoot you in the air. */
            ConfigValue<bool> TurboSpeed = ConfigValue<bool>(this, "turbo_speed", false);

            /** Game will behave like spell wizard eye is casted and it will never expire. */
            ConfigValue<bool> WizardEye = ConfigValue<bool>(this, "wizard_eye", false);

            /** Activate debug HUD which show FPS and various other realtime debug information */
            ConfigValue<bool> ShowFPS = ConfigValue<bool>(this, "show_fps", false);

            /** Face pointed by mouse will flash with red for buildings or green for indoor. */
            ConfigValue<bool> ShowPickedFace = ConfigValue<bool>(this, "show_picked_face", false);

            /** Skip intro movie on startup */
            ConfigValue<bool> NoIntro = ConfigValue<bool>(this, "no_intro", false);

            /** Skip 3do logo on startup */
            ConfigValue<bool> NoLogo = ConfigValue<bool>(this, "no_logo", false);

            /** Don't play any sounds. Currently it doesn't affect in-house movies. */
            ConfigValue<bool> NoSound = ConfigValue<bool>(this, "no_sound", false);

            /** Don't play any movies. */
            ConfigValue<bool> NoVideo = ConfigValue<bool>(this, "no_video", false);

            /** Disable all actors */
            ConfigValue<bool> NoActors = ConfigValue<bool>(this, "no_actors", false);

            /** Disable all incoming damage to party. */
            ConfigValue<bool> NoDamage = ConfigValue<bool>(this, "no_damage", false);

            /** Disable all decorations */
            ConfigValue<bool> NoDecorations = ConfigValue<bool>(this, "no_decorations", false);

            /** Disable Margareth's tour messages on Emerald Island. */
            ConfigValue<bool> NoMargareth = ConfigValue<bool>(this, "no_margareth", false);

            /** Verbose logging to debug console. Can be extremely spammy. */
            ConfigValue<bool> VerboseLogging = ConfigValue<bool>(this, "verbose_logging", false);
        };

        Debug debug{ this };

        class Gameplay : public ConfigSection {
         public:
            explicit Gameplay(GameConfig *config): ConfigSection(config, "gameplay") {}

            /** Use condition priorities from Grayface patches (e.g. Zombie has the lowest priority). */
            ConfigValue<bool> AlternativeConditionPriorities = ConfigValue<bool>(this, "alternative_condition_priorities", true);

            /** Artifact limit after which artifacts are no longer generated in loot. 0 - disable limit. */
            ConfigValue<int> ArtifactLimit = ConfigValue<int>(this, "artifact_limit", 13, &ValidateArtifactLimit);

            /** There are could be situations of item loss especially in high-level chests due to chest grid-limitations
              * 0 - Vanilla behaviour, items will be lost.
              * 1 - Try to place previously non-fit items on every chest opening.
              * 2 - Try to place previously non-fit items on every item pickup from the chest. */
            ConfigValue<int> ChestTryPlaceItems = ConfigValue<int>(this, "chest_try_place_items", 2);

            /** Maximum allowed slack for point-inside-a-polygon checks when calculating floor z level.
              * This is needed because there are actual holes in level geometry sometimes, up to several units wide. */
            ConfigValue<int> FloorChecksEps = ConfigValue<int>(this, "floor_checks_eps", 3, &ValidateFloorChecksEps);

            /** Gravity strength, the higher the more gravity, 0 - disable gravity completely. */
            ConfigValue<int> Gravity = ConfigValue<int>(this, "gravity", 5);

            /** Maximum depth for item pickup / opening chests / activating levers / etc with a keyboard (by pressing trigger key). */
            ConfigValue<float> KeyboardInteractionDepth = ConfigValue<float>(this, "keyboard_interaction_depth", 512.0f, &ValidateInteractionDepth);

            /** Minimum recovery time for melee weapons. 30 - vanilla */
            ConfigValue<int> MinRecoveryMelee = ConfigValue<int>(this, "minimum_recovery_melee", 30, &ValidateRecovery);

            /** Minimum recovery time for ranged weapons. 0 - vanilla, 5 - GrayFace patches */
            ConfigValue<int> MinRecoveryRanged = ConfigValue<int>(this, "minimum_recovery_ranged", 5, &ValidateRecovery);

            /** Minimum recovery time for blasters. 0 - vanilla, 5 - Grayface patches */
            ConfigValue<int> MinRecoveryBlasters = ConfigValue<int>(this, "minimum_recovery_blasters", 5, &ValidateRecovery);

            /** Maximum height which you can go with fly spell */
            ConfigValue<int> MaxFlightHeight = ConfigValue<int>(this, "max_flight_height", 4000, &ValidateMaxFlightHeight);

            /** Maximum depth at which right clicking on a monster produces a popup.
              * Also somehow this is the max depth for the souldrinker spell. */
            ConfigValue<float> MouseInfoDepthIndoor = ConfigValue<float>(this, "mouse_info_depth_indoor", 16192.0f, &ValidateInteractionDepth);
            ConfigValue<float> MouseInfoDepthOutdoor = ConfigValue<float>(this, "mouse_info_depth_outdoor", 12800.0f, &ValidateInteractionDepth); // That's 25 * 512, so 25 cells.

            /** Maximum depth for item pickup / opening chests / activating levers / etc with a mouse. */
            ConfigValue<float> MouseInteractionDepth = ConfigValue<float>(this, "mouse_interaction_depth", 512.0f, &ValidateInteractionDepth);

            ConfigValue<int> NewGameFood = ConfigValue<int>(this, "new_game_food", 7);
            ConfigValue<int> NewGameGold = ConfigValue<int>(this, "new_game_gold", 200);

            ConfigValue<int> PartyEyeLevel = ConfigValue<int>(this, "party_eye_level", 160);
            ConfigValue<int> PartyHeight = ConfigValue<int>(this, "party_height", 192);
            ConfigValue<int> PartyWalkSpeed = ConfigValue<int>(this, "party_walk_speed", 384);

            /** Max depth for ranged attacks and ranged spells. It's impossible to target monsters that are further away
              * than this value. Incidentally this is also the depth at which status bar tips are displayed on mouse over. */
            ConfigValue<float> RangedAttackDepth = ConfigValue<float>(this, "ranged_attack_depth", 5120.0f, &ValidateRangedAttackDepth);

            /** Show unidentified items in green mask in inventory, otherwise vanilla behaviour when green mask applied in shops only. */
            ConfigValue<bool> ShowUndentifiedItem = ConfigValue<bool>(this, "show_unidentified_item", false);

            /** New Game starting map */
            ConfigValue<std::string> StartingMap = ConfigValue<std::string>(this, "starting_map", "out01.odm");

            /** Treat clubs as maces. In vanilla clubs are using separate hidden skill and so equipable without learned Mace skill. */
            ConfigValue<bool> TreatClubAsMace = ConfigValue<bool>(this, "treat_club_as_mace", false);

         private:
            static int ValidateMaxFlightHeight(int max_flight_height) {
                if (max_flight_height <= 0 || max_flight_height > 16192)
                    return 4000;

                return max_flight_height;
            }
            static int ValidateArtifactLimit(int artifact_limit) {
                if (artifact_limit < 0)
                    return 0;

                return artifact_limit;
            }
            static float ValidateInteractionDepth(float depth) {
                return std::clamp(depth, 64.0f, 16192.0f);
            }
            static float ValidateRangedAttackDepth(float depth) {
                return std::clamp(depth, 64.0f, 16192.0f);
            }
            static int ValidateFloorChecksEps(int eps) {
                return std::clamp(eps, 0, 10);
            }
            static int ValidateRecovery(int recovery) {
                if (recovery < 0)
                    return 0;

                return recovery;
            }
        };

        Gameplay gameplay{ this };

        class Graphics : public ConfigSection {
         public:
            explicit Graphics(GameConfig *config): ConfigSection(config, "graphics") {}

            ConfigValue<std::string> Renderer = ConfigValue<std::string>(this, "renderer", "OpenGL", &ValidateRenderer);

            /** Enable bloodsplats under corpses */
            ConfigValue<bool> BloodSplats = ConfigValue<bool>(this, "bloodsplats", true);

            /** Bloodsplats radius multiplier. */
            ConfigValue<float> BloodSplatsMultiplier = ConfigValue<float>(this, "bloodsplats_multiplier", 1.0f);

            /** Do Bloodsplats fade. */
            ConfigValue<bool> BloodSplatsFade = ConfigValue<bool>(this, "bloodsplats_fade", true);

            ConfigValue<float> ClipFarDistance = ConfigValue<float>(this, "clip_far_distance", 16192.0f);
            ConfigValue<float> ClipNearDistance = ConfigValue<float>(this, "clip_near_distance", 32.0f);

            ConfigValue<bool> ColoredLights = ConfigValue<bool>(this, "colored_lights", true);

            /** D3D device number which was set by setup program in vanilla for hardware mode. */
            ConfigValue<int> D3DDevice = ConfigValue<int>(this, "d3d_device", 0);

            // lightmap builder option for old drawing system
            /** Need to be eventually deleted and replaced with gamma? */
            ConfigValue<bool> DynamicBrightness = ConfigValue<bool>(this, "dynamic_brightness", true);

            /** Disable fog effect - at far clip and on fog weather */
            ConfigValue<bool> Fog = ConfigValue<bool>(this, "fog", true);

            /** Adjusts fog height for bottom sky horizon */
            ConfigValue<int> FogHorizon = ConfigValue<int>(this, "fog_horizon", 39);

            /** Adjust starting depth ratio of distance fog */
            ConfigValue<float> FogDepthRatio = ConfigValue<float>(this, "fog_ratio", 0.75f);

            /** FPS Limit */
            ConfigValue<int> FPSLimit = ConfigValue<int>(this, "fps_limit", 60);

            /** Game level brightness gamma */
            ConfigValue<int> Gamma = ConfigValue<int>(this, "gamma", 4, &ValidateGamma);

            /** Viewport top-left offset */
            ConfigValue<int> HouseMovieX1 = ConfigValue<int>(this, "house_movie_x1", 8);
            ConfigValue<int> HouseMovieY1 = ConfigValue<int>(this, "house_movie_y1", 8);

            /** Viewport bottom-right offset */
            ConfigValue<int> HouseMovieX2 = ConfigValue<int>(this, "house_movie_x2", 172);
            ConfigValue<int> HouseMovieY2 = ConfigValue<int>(this, "house_movie_y2", 128);

            /** Use low-resolution bitmaps from HWL file instead of hi-resolution ones from LOD. */
            ConfigValue<bool> HWLBitmaps = ConfigValue<bool>(this, "hwl_bitmaps", false);

            /** Use low-resolution sprites from HWL file instead of hi-resolution ones from LOD. */
            ConfigValue<bool> HWLSprites = ConfigValue<bool>(this, "hwl_sprites", false);

            /** Max number of BSP sectors to display. */
            ConfigValue<int> MaxVisibleSectors = ConfigValue<int>(this, "maxvisiblesectors", 10, &ValidateMaxSectors);

            /** Allow changing trees/ground depending on current season (originally was only used in MM6) */
            ConfigValue<bool> SeasonsChange = ConfigValue<bool>(this, "seasons_change", true);

            /** Snow effect from MM6 where it was activated by event. Currently it shows every third day in winter. */
            ConfigValue<bool> Snow = ConfigValue<bool>(this, "snow", false);

            /** Vanilla's monster coloring method from hardware mode. When monsters look like bucket of paint was thrown at them. */
            ConfigValue<bool> Tinting = ConfigValue<bool>(this, "tinting", false);

            /** Torchlight distance per each power level. 0 - disable torchlight. */
            ConfigValue<int> TorchlightDistance = ConfigValue<int>(this, "torchlight_distance", 800, &ValidateTorchlight);

            /** Torchlight lighting flicker effect distance. 0 - disable effect. */
            ConfigValue<int> TorchlightFlicker = ConfigValue<int>(this, "torchlight_flicker", 200, &ValidateTorchlight);

            /** Enable synchronization of framerate with monitor vertical refresh rate. */
            ConfigValue<bool> VSync = ConfigValue<bool>(this, "vsync", false);

            /** Viewport top-left offset */
            ConfigValue<int> ViewPortX1 = ConfigValue<int>(this, "viewport_x1", 8);
            ConfigValue<int> ViewPortY1 = ConfigValue<int>(this, "viewport_y1", 8);

            /** Viewport bottom-right offset */
            ConfigValue<int> ViewPortX2 = ConfigValue<int>(this, "viewport_x2", 172);
            ConfigValue<int> ViewPortY2 = ConfigValue<int>(this, "viewport_y2", 128);

         private:
            static std::string ValidateRenderer(std::string renderer) {
                if (renderer != "OpenGL"/* && renderer != "DirectDraw"*/)
                    renderer = "OpenGL";

                return renderer;
            }
            static int ValidateGamma(int level) {
                return std::clamp(level, 0, 9);
            }
            static int ValidateMaxSectors(int sectors) {
                return std::clamp(sectors, 1, 150);
            }
            static int ValidateTorchlight(int distance) {
                if (distance < 0)
                    return 0;

                return distance;
            }
        };

        Graphics graphics{ this };

        class Keybindings : public ConfigSection {
         public:
            explicit Keybindings(GameConfig *config) : ConfigSection(config, "keybindings") {}

            ConfigValue<std::string> AlwaysRun = ConfigValue<std::string>(this, "always_run", "U", &ValidateKey);
            ConfigValue<std::string> Attack = ConfigValue<std::string>(this, "attack", "A", &ValidateKey);
            ConfigValue<std::string> AutoNotes = ConfigValue<std::string>(this, "auto_notes", "N", &ValidateKey);
            ConfigValue<std::string> Backward = ConfigValue<std::string>(this, "backward", "DOWN", &ValidateKey);
            ConfigValue<std::string> Cast = ConfigValue<std::string>(this, "cast", "C", &ValidateKey);
            ConfigValue<std::string> CastReady = ConfigValue<std::string>(this, "cast_ready", "S", &ValidateKey);
            ConfigValue<std::string> CenterView = ConfigValue<std::string>(this, "center_view", "END", &ValidateKey);
            ConfigValue<std::string> CharCycle = ConfigValue<std::string>(this, "char_cycle", "TAB", &ValidateKey);
            ConfigValue<std::string> Combat = ConfigValue<std::string>(this, "combat", "RETURN", &ValidateKey);
            ConfigValue<std::string> EventTrigger = ConfigValue<std::string>(this, "event_trigger", "SPACE", &ValidateKey);
            ConfigValue<std::string> FlyDown = ConfigValue<std::string>(this, "fly_down", "INSERT", &ValidateKey);
            ConfigValue<std::string> FlyUp = ConfigValue<std::string>(this, "fly_up", "PAGE UP", &ValidateKey);
            ConfigValue<std::string> Forward = ConfigValue<std::string>(this, "forward", "UP", &ValidateKey);
            ConfigValue<std::string> Jump = ConfigValue<std::string>(this, "jump", "X", &ValidateKey);
            ConfigValue<std::string> Land = ConfigValue<std::string>(this, "land", "HOME", &ValidateKey);
            ConfigValue<std::string> Left = ConfigValue<std::string>(this, "left", "LEFT", &ValidateKey);
            ConfigValue<std::string> LookDown = ConfigValue<std::string>(this, "look_down", "DELETE", &ValidateKey);
            ConfigValue<std::string> LookUp = ConfigValue<std::string>(this, "look_up", "PAGE DOWN", &ValidateKey);
            ConfigValue<std::string> MapBook = ConfigValue<std::string>(this, "map_book", "M", &ValidateKey);
            ConfigValue<std::string> Pass = ConfigValue<std::string>(this, "pass", "B", &ValidateKey);
            ConfigValue<std::string> Quest = ConfigValue<std::string>(this, "quest", "Q", &ValidateKey);
            ConfigValue<std::string> QuickReference = ConfigValue<std::string>(this, "quick_reference", "Z", &ValidateKey);
            ConfigValue<std::string> Rest = ConfigValue<std::string>(this, "rest", "R", &ValidateKey);
            ConfigValue<std::string> Right = ConfigValue<std::string>(this, "right", "RIGHT", &ValidateKey);
            ConfigValue<std::string> StepLeft = ConfigValue<std::string>(this, "step_left", "L BRACKET", &ValidateKey);
            ConfigValue<std::string> StepRight = ConfigValue<std::string>(this, "step_right", "R BRACKET", &ValidateKey);
            ConfigValue<std::string> TimeCalendar = ConfigValue<std::string>(this, "time_calendar", "T", &ValidateKey);
            ConfigValue<std::string> Yell = ConfigValue<std::string>(this, "yell", "Y", &ValidateKey);
            ConfigValue<std::string> ZoomIn = ConfigValue<std::string>(this, "zoom_in", "ADD", &ValidateKey);
            ConfigValue<std::string> ZoomOut = ConfigValue<std::string>(this, "zoom_out", "SUBTRACT", &ValidateKey);

         private:
            static std::string ValidateKey(std::string key) {
                // TODO [config]: validation, maybe via GameKey::displayNames
                return key;
            }
        };

        Keybindings keybindings{ this };

        class Settings : public ConfigSection {
         public:
            explicit Settings(GameConfig *config) : ConfigSection(config, "settings") {}

            /** true - run, false - walk */
            ConfigValue<bool> AlwaysRun = ConfigValue<bool>(this, "always_run", true);

            /** Horizontal view flip upon exiting buildings */
            ConfigValue<bool> FlipOnExit = ConfigValue<bool>(this, "flip_on_exit", false);

            /** Show hits status in status bar */
            ConfigValue<bool> ShowHits = ConfigValue<bool>(this, "show_hits", true);

            /** Music volume level */
            ConfigValue<int> MusicLevel = ConfigValue<int>(this, "music_level", 3, &ValidateLevel);

            /** Sound volume level */
            ConfigValue<int> SoundLevel = ConfigValue<int>(this, "sound_level", 4, &ValidateLevel);

            /** Voice volume level */
            ConfigValue<int> VoiceLevel = ConfigValue<int>(this, "voice_level", 5, &ValidateLevel);

            /** Last saved screenshot number */
            ConfigValue<int> ScreenshotNumber = ConfigValue<int>(this, "screenshot_number", 0);

            /** Discrete turn speed, 0 - smooth, x16 - 64 degress turn, x32 - 128 degress turn.
              * Only smooth is usable on modern machines. */
            ConfigValue<float> TurnSpeed = ConfigValue<float>(this, "turn_speed", 0.0f, &ValidateTurnSpeed);

            /** Discrete vertical turn speed. */
            ConfigValue<int> VerticalTurnSpeed = ConfigValue<int>(this, "vertical_turn_speed", 25, &ValidateVerticalTurnSpeed);

            /** Party footstep's sound while moving. */
            ConfigValue<bool> WalkSound = ConfigValue<bool>(this, "walk_sound", true);

         private:
            static int ValidateLevel(int level) {
                return std::clamp(level, 0, 9);
            }
            static int ValidateVerticalTurnSpeed(int speed) {
                return std::clamp(speed, 1, 128);
            }
            static float ValidateTurnSpeed(float speed) {
                return std::clamp(speed, 0.0f, 1024.0f);
            }
        };

        Settings settings{ this };

        class Window : public ConfigSection {
         public:
            explicit Window(GameConfig *config): ConfigSection(config, "window") {}

            ConfigValue<std::string> Title = ConfigValue<std::string>(this, "title", "World of Might and Magic", &ValidateTitle);

            /** Display number as exposed by SDL. Order is platform-specific, e.g. on windows 0 is main display */
            ConfigValue<int> Display = ConfigValue<int>(this, "display", 0);

            /** Window mode. 0 - window, 1 - borderless window, 2 - fullscreen, 3 - borderless fullscreen. */
            ConfigValue<int> Mode = ConfigValue<int>(this, "mode", 0, &ValidateMode);

            /** Coordinates in pixels for position of left-top window corner. -1 is window centered on this axis. */
            ConfigValue<int> PositionX = ConfigValue<int>(this, "position_x", -1, &ValidatePosition);
            ConfigValue<int> PositionY = ConfigValue<int>(this, "position_y", -1, &ValidatePosition);

            /** Window size in pixels. */
            ConfigValue<int> Width = ConfigValue<int>(this, "width", 640, &ValidateWidth);
            ConfigValue<int> Height = ConfigValue<int>(this, "height", 480, &ValidateHeight);

            /** Grab mouse. When true you cannot move the mouse outside the game window while it is in focus. */
            ConfigValue<bool> MouseGrab = ConfigValue<bool>(this, "mouse_grab", true);

         private:
             static std::string ValidateTitle(std::string title) {
                 if (title.empty())
                     title = "World of Might and Magic";

                 return title;
             }
             static int ValidateMode(int mode) {
                 if (mode < 0 || mode > 3)
                     mode = 0;

                 return mode;
             }
             static int ValidatePosition(int position) {
                if (position < -1)
                    return -1;

                return position;
            }
            static int ValidateWidth(int dimension) {
                if (dimension < 640)
                    return 640;

                return dimension;
            }
            static int ValidateHeight(int dimension) {
                if (dimension < 480)
                    return 480;

                return dimension;
            }
        };

        Window window{ this };

     private:
        const std::string config_file = "womm.ini";
        Logger *logger = nullptr;
    };

}  // namespace Application
