/*****************************************************************************\
     Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
                This file is licensed under the Snes9x License.
   For further information, consult the LICENSE file in the root directory.
\*****************************************************************************/

/***********************************************************************************
  SNES9X for Mac OS (c) Copyright John Stiles

  Snes9x for Mac OS X

  (c) Copyright 2001 - 2011  zones
  (c) Copyright 2002 - 2005  107
  (c) Copyright 2002         PB1400c
  (c) Copyright 2004         Alexander and Sander
  (c) Copyright 2004 - 2005  Steven Seeger
  (c) Copyright 2005         Ryan Vogt
  (c) Copyright 2019         Michael Donald Buckley
 ***********************************************************************************/


#ifndef _mac_os_h_
#define _mac_os_h_

#import <Cocoa/Cocoa.h>

#import <os/lock.h>

#import "mac-controls.h"

enum
{
	kDrawingReserved1 = 1, // unused
	kDrawingOpenGL,
	kDrawingBlitGL
};

enum
{
	kWindowControllers = 0,
	kWindowPreferences,
	kWindowAbout,
	kWindowAutoFire,
	kWindowRomInfo,
	kWindowCheatFinder,
	kWindowKeyConfig,
	kWindowCheatEntry,
	kWindowScreen,
	kWindowServer,
	kWindowClient,
	kWindowExtra,
	kWindowSoundEffect,
	kWindowCoreImageFilter,
	kWindowMultiCart,

	kWindowCount
};

enum
{
	SNES_MULTIPLAYER5,
	SNES_MULTIPLAYER5_2,
	SNES_JOYPAD,
	SNES_MOUSE_SWAPPED,
	SNES_MOUSE,
	SNES_SUPERSCOPE,
	SNES_JUSTIFIER,
	SNES_JUSTIFIER_2,
	SNES_MAX_CONTROLLER_OPTIONS
};

enum
{
	VIDEOMODE_BLOCKY,
	VIDEOMODE_TV,
	VIDEOMODE_SMOOTH,
	VIDEOMODE_BLEND,
	VIDEOMODE_SUPEREAGLE,
	VIDEOMODE_2XSAI,
	VIDEOMODE_SUPER2XSAI,
	VIDEOMODE_EPX,
	VIDEOMODE_HQ2X,
	VIDEOMODE_HQ3X,
	VIDEOMODE_HQ4X,
	VIDEOMODE_NTSC_C,
	VIDEOMODE_NTSC_S,
	VIDEOMODE_NTSC_R,
	VIDEOMODE_NTSC_M,
	VIDEOMODE_NTSC_TV_C,
	VIDEOMODE_NTSC_TV_S,
	VIDEOMODE_NTSC_TV_R,
	VIDEOMODE_NTSC_TV_M
};

typedef struct
{
	long long	nextTime[12];
	uint16		buttonMask;
	uint16		toggleMask;
	uint16		tcMask;
	uint16		invertMask;
	SInt32		frequency;
}	AutoFireState;

typedef struct
{
	bool8		benchmark;
	bool8		glForceNoTextureRectangle;
	bool8		glUseClientStrageApple;
	bool8		glUseTexturePriority;
	int			glStorageHint;
}	ExtraOption;

#define kMacWindowHeight	(SNES_HEIGHT_EXTENDED)
#define MAC_MAX_CHEATS      150

extern volatile bool8	running, s9xthreadrunning;
extern volatile bool8	eventQueued, windowExtend;
extern volatile int		windowResizeCount;
extern uint32			controlPad[MAC_MAX_PLAYERS];
extern uint8			romDetect, interleaveDetect, videoDetect, headerDetect;
extern WindowRef		gWindow;
extern uint32			glScreenW, glScreenH;
extern CGRect			glScreenBounds;
extern Point			windowPos[kWindowCount];
extern CGSize			windowSize[kWindowCount];
extern CGImageRef		macIconImage[118];
extern int				macPadIconIndex, macLegendIconIndex, macMusicBoxIconIndex, macFunctionIconIndex;
extern int				macFrameSkip;
extern int32			skipFrames;
extern int64			lastFrame;
extern unsigned long	spcFileCount, pngFileCount;
extern bool8			finished, cartOpen,
						autofire, hidExist, directDisplay;
extern bool8			fullscreen, autoRes,
						glstretch, gl32bit, vsync, drawoverscan, lastoverscan, screencurvature,
						ciFilterEnable;
extern long				drawingMethod;
extern int				videoMode;
extern SInt32			macSoundVolume;
extern uint32			macSoundBuffer_ms, macSoundInterval_ms;
extern bool8			macSoundLagEnable;
extern uint16			aueffect;
extern uint8			saveInROMFolder;
extern NSString			*saveFolderPath;
extern int				macCurvatureWarp, macAspectRatio;
extern int				macFastForwardRate, macFrameAdvanceRate;
extern int				inactiveMode;
extern bool8			macQTRecord;
extern uint16			macRecordFlag, macPlayFlag, macQTMovFlag;
extern bool8			startopendlog, showtimeinfrz, enabletoggle, savewindowpos, onscreeninfo;
extern int				musicboxmode;
extern bool8			applycheat;
extern int				padSetting, deviceSetting, deviceSettingMaster;
extern int				macControllerOption;
extern CGPoint			unlimitedCursor;
extern char				npServerIP[256], npName[256];
extern AutoFireState	autofireRec[MAC_MAX_PLAYERS];
extern ExtraOption		extraOptions;
extern CFStringRef		multiCartPath[2];

#ifdef MAC_PANTHER_SUPPORT
extern IconRef			macIconRef[118];
#endif

extern bool8			pressedKeys[MAC_MAX_PLAYERS][kNumButtons];
extern bool8            pressedGamepadButtons[MAC_MAX_PLAYERS][kNumButtons];
extern os_unfair_lock	keyLock;

extern NSOpenGLView		*s9xView;

void AdjustMenus (void);
void UpdateMenuCommandStatus (Boolean);
void ApplyNSRTHeaderControllers (void);
void QuitWithFatalError (NSString *);
void ChangeInputDevice (void);
void GetGameScreenPointer (int16 *, int16 *, bool);
void PostQueueToSubEventLoop (void);
int PromptFreezeDefrost (Boolean);
uint64 GetMicroseconds(void);

void CopyPressedKeys(uint8 keys[MAC_MAX_PLAYERS][kNumButtons], uint8 gamepadButtons[MAC_MAX_PLAYERS][kNumButtons]);

@interface S9xJoypad : NSObject
@property (nonatomic, assign) uint32 vendorID;
@property (nonatomic, assign) uint32 productID;
@property (nonatomic, assign) uint8 index;
@property (nonatomic, copy) NSString *name;
@end

@interface S9xJoypadInput : NSObject
@property (nonatomic, assign) uint32 cookie;
@property (nonatomic, assign) int32 value;
@property (nonatomic, assign) S9xButtonCode buttonCode;
@end

@protocol S9xInputDelegate <NSObject>
- (BOOL)handleInput:(S9xJoypadInput *)input fromJoypad:(S9xJoypad *)joypad;
@end

extern id<S9xInputDelegate> inputDelegate;

@interface S9xEngine : NSObject

@property (nonatomic, weak) id<S9xInputDelegate> inputDelegate;

- (void)start;
- (void)stop;

- (BOOL)isRunning;
- (BOOL)isPaused;
- (void)pause;
- (void)resume;

- (BOOL)setButton:(S9xButtonCode)button forKey:(int16)key player:(int8)player oldButton:(S9xButtonCode *)oldButton oldPlayer:(int8 *)oldPlayer oldKey:(int16 *)oldKey;
- (void)clearButton:(S9xButtonCode)button forPlayer:(int8)player;

- (NSArray<S9xJoypad *> *)listJoypads;
- (void)setPlayer:(int8)player forVendorID:(uint32)vendorID productID:(uint32)productID index:(uint32)index oldPlayer:(int8 *)oldPlayer;
- (BOOL)setButton:(S9xButtonCode)button forVendorID:(uint32)vendorID productID:(uint32)productID index:(uint32)index cookie:(uint32)cookie value:(int32)value oldButton:(S9xButtonCode *)oldButton;
- (void)clearJoypadForVendorID:(uint32)vendorID productID:(uint32)productID index:(uint32)index;
- (void)clearJoypadForVendorID:(uint32)vendorID productID:(uint32)productID index:(uint32)index buttonCode:(S9xButtonCode)buttonCode;
- (NSArray<S9xJoypadInput *> *)getInputsForVendorID:(uint32)vendorID productID:(uint32)productID index:(uint32)index;

- (NSString *)labelForVendorID:(uint32)vendorID productID:(uint32)productID cookie:(uint32)cookie value:(int32)value;

- (BOOL)loadROM:(NSURL *)fileURL;

- (void)setVideoMode:(int)videoMode;
- (void)setShowFPS:(BOOL)showFPS;

@end

#endif
