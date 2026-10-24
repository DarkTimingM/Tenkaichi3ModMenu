#ifndef BTL_MOD_H
#define BTL_MOD_H

/* What the game's code asks the PC build's mod menu (F12; port/src/plat_stub.c, port/src/port_mod.h). The menu has
   profiles that match a fighter by who plays it (a person or the CPU), the game port it reads, its character and its
   form; BTL_MOD_PROFILE(chr) says which one is in effect (-1: none: the menu off, an online match, no match).
   With no profile in effect every answer is the game's own: window timing unchanged, 100 percent, 1 point. */

extern int BattleSide_IsCpu(int side); /* battle_load.c */

/* A fighter: +0 its side, +4 the game port it reads (BattleSide.pad), +0x994 the fighting member, the members
   from +0x9A4 in steps of 0xA4 (BtlMember: +0 character, +4 costume). */
#define BTL_CHR_SIDE(chr) (*(int *)(chr))
#define BTL_CHR_PORT(chr) (*(int *)((char *)(chr) + 4))

static int BtlMod_MemberField(void *chr, int off) {
    int at = *(int *)((char *)chr + 0x994);

    return at >= 0 && at < 8 ? *(int *)((char *)chr + 0x9A4 + at * 0xA4 + off) : -1;
}

extern int Port_ModResolve(int side, int pad, int cpu, int chara, int costume);

/* The profile in effect for a fighter, -1 when there is none or the fighter is not one a profile can name (no fighter,
   a side that is not 0 / 1). */
static int BtlMod_Profile(void *chr) {
    int side;

    if (chr == 0) {
        return -1;
    }
    side = BTL_CHR_SIDE(chr);
    if (side < 0 || side > 1) {
        return -1;
    }
    return Port_ModResolve(side, BTL_CHR_PORT(chr), BattleSide_IsCpu(side), BtlMod_MemberField(chr, 0), BtlMod_MemberField(chr, 4));
}
#define BTL_MOD_PROFILE(chr) BtlMod_Profile(chr)

#define BTL_MODWIN_COUNTER 0 /* the Up + Rush counter window (+0x1070): its length in frames (1 = the game's) */
#define BTL_MODWIN_VANISH 1  /* the evasion window (+0x1068): extra frames */
#define BTL_MODWIN_GUARD 2   /* the other windows a guard press opens (+0x106C, +0x1078, +0x107C): extra frames */
#define BTL_MODWIN_SIDE 3    /* the guard + side window (+0x1074): extra frames */
#define BTL_MODWIN_SMASH 4   /* the Perfect Smash: frames from the first full frame of a charged smash that still count (1 = the game's) */

#define BTL_MODPCT_MELEE 0
#define BTL_MODPCT_BLAST 1
#define BTL_MODPCT_SP1 2
#define BTL_MODPCT_SP2 3
#define BTL_MODPCT_ULT 4
#define BTL_MODPCT_KI 5
#define BTL_MODPCT_GAUGE 6
#define BTL_MODPCT_KIHIT 7 /* the ki a fighter gains for each hit it lands */
#define BTL_MODPCT_SUPERCOST 8 /* ki cost of attacks 1 and 2 */
#define BTL_MODPCT_ULTCOST 9   /* ki cost of the ultimate */
#define BTL_MODPCT_BLASTCOST 10 /* ki cost of ki blasts */
#define BTL_MODPCT_MOVECOST 11 /* ki drained by dashes and the like, per frame */
#define BTL_MODPCT_KIREGEN 12  /* passive ki regeneration */
#define BTL_MODPCT_TAKEN 13    /* damage taken */
#define BTL_MODPCT_SPEED 14    /* movement speed */
#define BTL_MODPCT_BLASTLIMIT 15 /* how many ki blasts a fighter may fire in a row and have alive at once */
#define BTL_MODPCT_BLASTSPEED 16 /* ki blast speed */

#define BTL_MODFLAG_FREESUPER 0 /* attacks 1 and 2 cost no ki */
#define BTL_MODFLAG_FREEULT 1   /* the ultimate costs no ki */
#define BTL_MODFLAG_INFKI 2     /* ki is never spent */

/* All take the profile BTL_MOD_PROFILE gave. */
extern int Port_ModWindow(int profile, int which);
extern int Port_ModLenient(int profile, int which); /* 1 when that window has a level above 0: every press re-opens it */
extern int Port_ModPercent(int profile, int what);
extern int Port_ModClashPoints(int profile, int side); /* what one accepted input is worth in a clash (1 = the game's) */
extern int Port_ModClashAny(int profile);              /* 1: any button counts as an input in a clash */
extern int Port_ModClashStart(int profile);            /* points added to a beam clash's starting score */
extern int Port_ModFlag(int profile, int what);        /* BTL_MODFLAG_*: 0 / 1 */

#endif
