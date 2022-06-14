#define SystemSevenOrLater 1
#include <systypes.r>
#include <Types.r>

resource 'MENU' (500) {
	500,
	textMenuProc,
	allEnabled,
	enabled,
	apple,
	{	
		"About mShow...", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (501) {
	501,
	textMenuProc,
	0x7FFFDBB7,
	enabled,
	"File",
	{	
		"New", noIcon, "N", noMark, plain,
		"Open...", noIcon, "O", noMark, plain,
		"Close", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Save", noIcon, "S", noMark, plain,
		"Save as...", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Page Setup...", noIcon, noKey, noMark, plain,
		"Print...", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (502) {
	502,
	textMenuProc,
	0x7FFFFFBC,
	enabled,
	"Edit",
	{	
		"Undo", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain,
		"Cut", noIcon, "X", noMark, plain,
		"Copy", noIcon, "C", noMark, plain,
		"Paste", noIcon, "V", noMark, plain,
		"Clear", noIcon, noKey, noMark, plain,
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (5003) {
	5003,
	textMenuProc,
	allEnabled,
	enabled,
	"Windows",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (2000) {
	2000,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2001) {
	2001,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2002) {
	2002,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2003) {
	2003,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2004) {
	2004,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2005) {
	2005,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2006) {
	2006,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2007) {
	2007,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2008) {
	2008,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2009) {
	2009,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2010) {
	2010,
	textMenuProc,
	allEnabled,
	enabled,
	"Pop",
	{	
		"ONE", noIcon, noKey, noMark, plain
	}
};
resource 'ALRT' (505) {
 {50, 100, 150, 410},
 505,
	{	
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1
	},
	noAutoCenter
};

resource 'ALRT' (600) {
 {50, 100, 150, 410},
 600,
	{	
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1
	},
	noAutoCenter
};

resource 'ALRT' (4505) {
 {50, 100, 150, 410},
4505,
	{	
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1
	},
	noAutoCenter
};

resource 'ALRT' (4999) {
 {50, 100, 150, 410},
 4999,
	{	
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1,
		OK, visible, sound1
	},
	noAutoCenter
};

resource 'DITL' (505) {
	{	
		{60, 50, 80, 125},
		Button {
			enabled,
			"Save"
		},
		{60, 135, 80, 210},
		Button {
			enabled,
			"Discard"
		},
		{60, 220, 80, 300},
		Button {
			enabled,
			"Cancel"
		},
		{10, 80, 50, 285},
		StaticText {
			enabled,
			"    ^0"
		}
	}
};

resource 'DITL' (600) {
	{	
		{65, 135, 85, 210},
		Button {
			enabled,
			"Ok"
		},
		{10, 80, 60, 285},
		StaticText {
			enabled,
			"    ^0"
		}
	}
};

resource 'DITL' (4505) {
	{	
		{60, 50, 80, 125},
		Button {
			enabled,
			"Ok"
		},
		{60, 135, 80, 210},
		Button {
			enabled,
			"Continue"
		},
		{10, 80, 50, 285},
		StaticText {
			enabled,
			"    ^0"
		}
	}
};


resource 'DITL' (4999) {
	{	
		{60, 50, 80, 125},
		Button {
			enabled,
			"Ok"
		},
		{60, 135, 80, 210},
		Button {
			enabled,
			"Continue"
		},
		{10, 80, 50, 285},
		StaticText {
			enabled,
			"    ^0"
		}
	}
};


resource 'DLOG' (5667, purgeable)
{
	  {0 , 0, 186, 344}, dBoxProc, invisible, noGoAway , 0,
		5667, "", noAutoCenter
};

resource 'DITL' (5667)
{ {
	{135, 252, 155, 332}, Button { enabled, "Open" },
	{104, 252, 124, 332}, Button { enabled, "Cancel" },
	{0, 0, 0, 0}, HelpItem {disabled, HMScanhdlg {-6042}},
	{8, 235, 24, 337}, UserItem { enabled },
	{32, 252, 52, 332}, Button { enabled, "Eject"},
	{60, 252, 80, 332}, Button { enabled, "Disktop"},
	{29, 12, 159, 230}, UserItem { enabled},
	{6, 12, 25, 230}, UserItem { enabled},
	{91, 251, 92, 333}, Picture { disabled, 11},
	{160, 12, 180, 230}, Button { enabled, "Start Here" },
} };


resource 'vers' (1, purgeable)
{
     0x12, 0x80, release, 0x00, verUS,
    "16.7",
    "16.7 Copyright © SAIC,1993-2007"
};

