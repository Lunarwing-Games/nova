#ifndef STEAM_MGR_H
#define STEAM_MGR_H

// Self contained header for Steamworks, this abstracts processes and functions
// provided by Valve Software for ease of use in the project
//
// This way, if someone wants to exclude Steamworks, there doesn't need to be
// any edits to the existing codebase!

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// You do not need to #define this yourself, this is done via CMake!
#ifndef BUILD_WITH_STEAMWORKS

// Turn all the cool steam funcs/macros into no-ops in case we don't want it
// or if we don't have it, whatever the situ is at the moment
//
// Or it returns exactly what we need it to, to avoid UB

bool init_steamworks() { return true; }
void shutdown_steamworks() { return ; }
const char *steam_get_username() { return "PLAYER"; }
uint64_t steam_get_steamid() { return (uint64_t)0xFFFF; }
bool unlock_achevement(const char *id) { return true; }
bool is_achievement_unlocked(const char *id) { return true; }
bool steam_upload_file(const char *filename, const void *data, size_t size) { return false; }
bool steam_download_file(const char *filename, const void *out_buf, size_t size) { return false; }

#else

// If the macro is defined, then we fill out all the functionality needed for
// the project. Refer to the Steamworks documentation for info on SteamAPI!
//

    #include <steam/steam_api.h>

    bool init_steamworks() {
        if(!SteamAPI_Init()) return false;
        return true;
    }

    void shutdown_steamworks() {
        SteamAPI_Shutdown();
        return;
    }

    const char *steam_get_username() {
        if(SteamFriends()) {
            return SteamFriends()->GetPersonaName();
        } else {
            return "PLAYER";
        }
    }

    uint64_t steam_get_steamid() {
        if(SteamUser()) {
            return SteamUser()->GetSteamID().ConvertToUint64();
        } else {
            return 0ULL;
        }
    }

    bool unlock_achievement(const char *id) {
        if(!SteamUserStats())
            return false;
        
        bool success = SteamUserStats()->SetAchievement(id);
        SteamUserStats()->StoreStats();
        return success;
    }

    bool is_achievement_unlocked(const char *id) {
        if (!SteamUserStats())
        return false;

        bool achieved = false;
        SteamUserStats()->GetAchievement(id, &achieved);
        return achieved;
    }

    bool steam_upload_file(const char *filename, const void *data, size_t size) {
        fprintf(stderr, "Steam Cloud Saves are not implemented yet!\n");
        return false;
    }

    bool steam_download_file(const char *filename, const void *out_buf, size_t size) {
        fprintf(stderr, "Steam Cloud Saves are not implemented yet!\n");
        return false;
    }
#endif

#endif // steam_mgr.h