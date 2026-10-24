/*
 * The mod menu (F12, gs/ui.cpp): profiles that tune a fighter's defence windows, the damage of each kind of attack,
 * the charge rates and how easily it wins a clash. A profile applies to the fighters that match it: the people
 * playing, the CPU or both; the game port they read; the controller that drives them (its GUID or its name); and
 * one character (and one form of it). The most specific profile that matches wins.
 * The profiles are kept in the settings file (p01_* .. p12_*). Never in effect in an online match.
 * plat_stub.c has the state; include/battle/btl_mod.h is what the game's code calls.
 */
#ifndef PORT_MOD_H
#define PORT_MOD_H

#ifdef __cplusplus
extern "C" {
#endif

#define PORTMOD_PROFILES 12
#define PORTMOD_MAX_LEVEL 10 /* timing levels: 0 the game's own windows .. 10 extremely easy */

#define PORTMOD_WHO_PLAYER 0 /* the people playing */
#define PORTMOD_WHO_CPU 1
#define PORTMOD_WHO_BOTH 2

typedef struct {
    int used;  /* the slot holds a profile */
    int on;    /* and it is in effect */
    char label[32];
    int who;   /* PORTMOD_WHO_* */
    int port;  /* -1 any game port, 0 / 1: only the fighter that reads that port */
    int useCtl; /* only the fighter a chosen controller drives */
    int match;  /* with useCtl: 0 the controller's GUID, 1 its name */
    int exact;  /* with useCtl: only the controller that was chosen, not another of the same model */
    int ordG, ordN; /* which of the identical ones it was: how many connected controllers before it had its GUID / its name */
    char guid[40];
    char name[64];
    int chara, costume; /* -1 any: one character (by its id in the character table) and one form (costume) of it */
    int tCounter, tVanish, tGuard, tSide, tSmash; /* timing levels 0..10 */
    int dMelee, dBlast, dSp1, dSp2, dUlt; /* damage dealt, percent 0..1000 (100 = the game's) */
    int kiCharge, gaugeGain, kiHit;       /* ki charge rate, skill (blast) gauge gain, ki per hit landed: percent */
    int cSuper, cUlt, cBlast, cMove;      /* ki costs, percent 0..1000: attacks 1 and 2, the ultimate, ki blasts, movement */
    int kiRegen;                          /* passive ki regeneration, percent 0..1000 */
    int taken;                            /* damage taken, percent 0..1000 */
    int speed;                            /* movement speed, percent 50..200 */
    int blastLimit;                       /* how many ki blasts in a row / alive at once, percent 100..500 */
    int blastSpeed;                       /* ki blast speed, percent 25..400 */
    int freeSuper, freeUlt, infKi;        /* attacks 1 and 2 cost no ki / the ultimate costs no ki / ki is never spent */
    int clashPow;   /* what each accepted input is worth in a clash, percent 100..1000 */
    int clashAny;   /* any button counts as an input in a clash (not only a direction) */
    int clashStart; /* points added to a beam clash's starting score 0..50 */
} PortModProfile;

/* The "Host" row of the controller settings: one controller that gets one profile, with a switch. It goes before the
   profiles' own conditions: a human fighter driven by that exact controller gets the chosen profile (if the profile names
   a character, only that one), whether or not the profile is switched on in the mod menu. */
typedef struct {
    int on;
    int managed; /* the row has been used: the profile it names is then in effect only through the row (the tick and the
                    controller), not through its own conditions */
    int profile; /* 0 .. PORTMOD_PROFILES - 1 */
    int ordG;    /* which of the identical controllers it is (see PortModProfile) */
    char guid[40];
    char name[64];
} PortModHost;

/* The last fighter the game asked about, per side (for the menu's "use this fighter" buttons). */
typedef struct {
    int seen, pad, cpu, chara, costume, profile;
} PortModSeen;

PortModProfile *PortMod_Get(int index); /* 0 .. PORTMOD_PROFILES - 1; read from the settings on first use */
void PortMod_Save(int index);
void PortMod_Reset(int index); /* the values back to the game's own (not saved until PortMod_Save) */
void PortMod_Clear(int index); /* empties the slot */
void PortMod_Create(int index); /* a new profile in the slot, in effect, for the people playing */
int PortMod_Frames(int which, int level); /* what a timing level means: frames (counter) or extra frames */
int PortMod_Resolve(int side, int pad, int cpu, int chara, int costume); /* the profile in effect for a fighter, -1 none */
const PortModSeen *PortMod_Seen(int side);
PortModHost *PortMod_Host(void);
void PortMod_HostSave(void);

#ifdef __cplusplus
}
#endif
#endif
