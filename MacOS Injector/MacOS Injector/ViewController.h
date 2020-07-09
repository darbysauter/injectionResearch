//
//  ViewController.h
//  MacOS Injector
//
//  Created by Mumphus on 2019-07-25.
//  Copyright © 2019 mumphus. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController <NSMenuDelegate>

@property (strong) IBOutlet NSPopUpButton *processSelector;
@property (strong) IBOutlet NSButton *selectFileButton;
@property (strong) IBOutlet NSTextField *selectedFileText;

- (IBAction)selectFilePressed:(id)sender;
- (IBAction)injectButtonPressed:(id)sender;
- (IBAction)processSelected:(id)sender;

@property NSURL *selectedFile;
@property int selectedPID;
@property NSMutableArray *PIDArray;

- (void)showFileSelector;

@end

