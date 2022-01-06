/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int snap      = 32;       /* snap pixel */
static const double inactiveopacity = 0.80;
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_forestgreen[] = "#008822";
static const char col_lightgreen[] = "#00ff22";
static const char col_darkforest[] = "#005816";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeTitle] = { col_lightgreen, col_gray1, col_gray2 },
	[SchemeSel]  = { col_lightgreen, col_darkforest,  col_darkforest  },
};

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, ALPHA,  OPAQUE},
	[SchemeTitle]  = { ALPHA, ALPHA,  OPAQUE},
	[SchemeSel]  = { OPAQUE, OPAQUE,  OPAQUE},
};

static const char *const autostart[] = {
       "/usr/lib/gnome-session/run-systemd-session", "dwm-unity-session.target",
       "dwmstatus", NULL,
       "compton", NULL,
       NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *      WM_CLASS(STRING) = instance, class
	 *      WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor opacity inactiveopacity*/
	{ "Gimp",       NULL,       NULL,       1 << 5,       0,           -1,     -1,     -1 },
	{ "Nitrogen",   NULL,       NULL,       0,            1,           -1,     -1,     -1 },
	{ "discord",    NULL,       NULL,       1 << 4,       0,           -1,     -1,     -1 },
	{ "Firefox",    NULL,       NULL,       1 << 8,       0,           -1,     0.95,   1.0 },
	{ "st",         NULL,       NULL,       0,            1,           -1,     -1,     1.0 },
	{ "Nextcloud",  NULL,       NULL,       1 << 7,       0,           -1,     -1,     -1 },
	{ "anbox",      NULL,       NULL,       1 << 6,       0,           -1,     -1,     -1 },
};

static const MonitorRule monrules[] = {
	/* monitor layout  mfact  nmaster  showbar  topbar  borderpx  defaultopacity */
	{  1,      0,      0.50,    -1,      1,      1,     1,         0.95     }, // use a different layout for the second monitor
	{  -1,     0,      0.55,    1,      1,      1,     1,         0.95     }, // default
};

/* layout(s) */
static const float cfact     = 1.0; /* factor of tiled clients [0.05..4.0] */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "(゜∀。)",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define AltMask Mod1Mask
#define NumLock Mod2Mask
#define TAGKEYS(KEY,TAG) \
	{ KeyPress,  MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ KeyPress,  MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ KeyPress,  MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ KeyPress,  MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dbash[] = { "dbash", "-m", dmenumon, "-fn", dmenufont, "-nb", col_forestgreen, "-nf", col_lightgreen, "-sb", col_lightgreen, "-sf", col_forestgreen, NULL };
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-H", ".bash_history", "-fn", dmenufont, "-nb", col_forestgreen, "-nf", col_lightgreen, "-sb", col_lightgreen, "-sf", col_forestgreen, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browser[]  = { "firefox", NULL };
static const char *lanu[] = { "dibus", NULL };

/* custom functions */
static const char emoji[130]="[ `ibus engine` = uniemoji ] && ibus engine `cat .local/dwm/ibus` || `ibus engine > .local/dwm/ibus && ibus engine uniemoji`";
static const char *toggleemoji[] = { "/bin/sh", "-c",emoji, NULL };
static const char *scrot[] = { "/bin/sh", "-c", "cd  `cat $HOME/.config/dwm/scrotdir`;scrot `cat $HOME/.config/dwm/scrotname`", NULL };
static const char *scrots[] = { "/bin/sh", "-c", "cd `cat $HOME/.config/dwm/scrotdir`;scrot -s", NULL };
static const char *scrotu[] = { "/bin/sh", "-c", "cd `cat $HOME/.config/dwm/scrotdir`;scrot -u", NULL };
static const char *voldec[] = { "/bin/sh", "-c", "pactl -- set-sink-volume 0 -10%", NULL };
static const char *volinc[] = { "/bin/sh", "-c", "pactl -- set-sink-volume 0 +10%", NULL };
static const char *volmut[] = { "/bin/sh", "-c", "amixer -D pulse set Master toggle", NULL };

static Key keys[] = {
	/* type         modifier                        key        function        argument */
	{ KeyPress,    MODKEY,                          XK_r,        spawn,          {.v = dmenucmd } },
	{ KeyPress,    MODKEY|ShiftMask,                XK_r,        spawn,          {.v = termcmd } },
	{ KeyPress,    MODKEY|ControlMask,              XK_r,        spawn,          {.v = dbash } },
	{ KeyPress,    MODKEY,                          XK_e,        spawn,          {.v = toggleemoji} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_l,        spawn,          {.v = lanu } },
	{ KeyPress,    MODKEY|ShiftMask,                XK_Return,   spawn,          {.v = browser } },
	{ KeyPress,    0,                               XK_Print,    spawn,          {.v = scrot } },
	{ KeyRelease,  ShiftMask,                       XK_Print,    spawn,          {.v = scrots } },
	{ KeyRelease,  ControlMask,                     XK_Print,    spawn,          {.v = scrotu } },
	{ KeyPress,    MODKEY|ControlMask,              XK_d,        spawn,          {.v = voldec} },
	{ KeyPress,    MODKEY|ControlMask,              XK_i,        spawn,          {.v = volinc} },
	{ KeyPress,    MODKEY|ControlMask,              XK_m,        spawn,          {.v = volmut} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_d,        setopacity,     {.f = -0.05} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_i,        setopacity,     {.f = +0.05} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_f,        setopacity,     {.f = 2.0} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_b,        spawn,          SHCMD("nitrogen --restore") },
	{ KeyPress,    MODKEY,                          XK_b,        togglebar,      {0} },
	{ KeyPress,    AltMask,                         XK_Tab,      focusstack,     {.i = +1 } },
	{ KeyPress,    AltMask|ShiftMask,               XK_Tab,      focusstack,     {.i = -1 } },
	{ KeyPress,    MODKEY,                          XK_i,        incnmaster,     {.i = +1 } },
	{ KeyPress,    MODKEY,                          XK_d,        incnmaster,     {.i = -1 } },
	{ KeyPress,    MODKEY,                          XK_k,        setcfact,       {.f = +0.10} },
	{ KeyPress,    MODKEY,                          XK_j,        setcfact,       {.f = -0.10} },
	{ KeyPress,    MODKEY,                          XK_h,        setmfact,       {.f = -0.05} },
	{ KeyPress,    MODKEY,                          XK_l,        setmfact,       {.f = +0.05} },
	{ KeyPress,    MODKEY,                          XK_Return,   zoom,           {0} },
	{ KeyPress,    MODKEY,                          XK_Tab,      view,           {0} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_Tab,      viewmon,        {0} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_c,        killclient,     {0} },
	{ KeyPress,    MODKEY,                          XK_t,        setlayout,      {.v = &layouts[0]} },
	{ KeyPress,    MODKEY,                          XK_f,        setlayout,      {.v = &layouts[1]} },
	{ KeyPress,    MODKEY,                          XK_m,        setlayout,      {.v = &layouts[2]} },
	{ KeyPress,    MODKEY,                          XK_space,    setlayout,      {0} },
	{ KeyPress,    MODKEY|ShiftMask,                XK_space,    togglefloating, {0} },
	{ KeyPress,    MODKEY,                          XK_0,        view,           {.ui = ~0 } },
	{ KeyPress,    MODKEY|ShiftMask,                XK_0,        tag,            {.ui = ~0 } },
	{ KeyPress,    MODKEY,                          XK_comma,    focusmon,       {.i = -1 } },
	{ KeyPress,    MODKEY,                          XK_period,   focusmon,       {.i = +1 } },
	{ KeyPress,    MODKEY|ShiftMask,                XK_comma,    tagmon,         {.i = -1 } },
	{ KeyPress,    MODKEY|ShiftMask,                XK_period,   tagmon,         {.i = +1 } },
	{ KeyPress,    MODKEY|ControlMask,              XK_comma,    seltagmon,      {.i = -1 } },
	{ KeyPress,    MODKEY|ControlMask,              XK_period,   seltagmon,      {.i = +1 } },
	{ KeyPress,    MODKEY|ShiftMask|ControlMask,    XK_d,        setborderpx,    {.i = -1 } },
	{ KeyPress,    MODKEY|ShiftMask|ControlMask,    XK_i,        setborderpx,    {.i = +1 } },
	{ KeyPress,    MODKEY|ShiftMask|ControlMask,    XK_f,        setborderpx,    {.i = 0 } },
	{ KeyPress,    MODKEY|ShiftMask|ControlMask,    XK_m,        setborderpx,    {.i = 2 } },
	TAGKEYS(                                        XK_1,                        0)
	TAGKEYS(                                        XK_2,                        1)
	TAGKEYS(                                        XK_3,                        2)
	TAGKEYS(                                        XK_4,                        3)
	TAGKEYS(                                        XK_5,                        4)
	TAGKEYS(                                        XK_6,                        5)
	TAGKEYS(                                        XK_7,                        6)
	TAGKEYS(                                        XK_8,                        7)
	TAGKEYS(                                        XK_9,                        8)
	{ KeyPress,    MODKEY|ShiftMask,                XK_q,        quit,           {0} },
};

static const char *powercontrol[] = { "dpower", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_forestgreen, "-sf", col_gray4, NULL };
static const char *dappo[] = { "dappo", NULL };
/* custom mouse functions */
static void switchmain(const Arg *arg);

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask                button          function        argument */
	{ ClkLtSymbol,          0,                        Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkLtSymbol,          0,                        Button2,        setlayout,      {.v = &layouts[1]} },
	{ ClkLtSymbol,          0,                        Button1,        setlayout,      {.v = &layouts[0]} },
	{ ClkStatusText,        0,                        Button3,        spawn,          {.v = volinc} },
	{ ClkStatusText,        0,                        Button2,        spawn,          {.v = volmut} },
	{ ClkStatusText,        0,                        Button1,        spawn,          {.v = voldec} },
	{ ClkWinTitle,          0,                        Button3,        spawn,          {.v = powercontrol } },
	{ ClkWinTitle,          MODKEY,                   Button3,        incnmaster,     {.i = +1 } },
	{ ClkWinTitle,          MODKEY,                   Button2,        spawn,          {.v = termcmd } },
	{ ClkWinTitle,          MODKEY,                   Button1,        incnmaster,     {.i = -1 } },
	{ ClkWinTitle,          0,                        Button2,        spawn,          {.v = dappo } },
	{ ClkWinTitle,          0,                        Button1,        spawn,          {.v = dmenucmd } },
	{ ClkClientWin,         MODKEY|ControlMask,       Button3,        tagmon,         {.i = +1 } },
	{ ClkClientWin,         MODKEY|ControlMask,       Button2,        tag,            {.ui = ~0 } },
	{ ClkClientWin,         MODKEY|ControlMask,       Button1,        tagmon,         {.i = -1 } },
	{ ClkClientWin,         MODKEY|ShiftMask,         Button3,        setopacity,     {.f = 1.0} },
	{ ClkClientWin,         MODKEY|ShiftMask,         Button2,        killclient,     {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,         Button1,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,                   Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY,                   Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,                   Button1,        movemouse,      {0} },
	{ ClkTagBar,            0,                        Button3,        switchmain,     {0} },
	{ ClkTagBar,            0,                        Button2,        view,           {0} },
	{ ClkTagBar,            0,                        Button1,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,                   Button3,        view,           {.ui = ~0 } },
	{ ClkTagBar,            MODKEY,                   Button2,        tag,            {0} },
	{ ClkTagBar,            MODKEY,                   Button1,        toggletag,      {0} },
};

void
switchmain(const Arg *arg) {
	Arg a = {0};

	view(arg);
	view(&a);
	toggleview(arg);
}
