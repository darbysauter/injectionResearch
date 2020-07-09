//
//  ViewController.m
//  MacOS Injector
//
//  Created by Mumphus on 2019-07-25.
//  Copyright © 2019 mumphus. All rights reserved.
//

#import "ViewController.h"
#import "procInfo.h"
#import "Utils.h"
#import "injection.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    _PIDArray = [NSMutableArray array];
    [[_processSelector menu] setDelegate:self];
    [_processSelector addItemWithTitle:@"Select Process"];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

- (void)menuWillOpen:(NSMenu *)menu {
    kern_return_t kern_return;
    mach_port_t task;
    
    [_processSelector removeAllItems];
    [_PIDArray removeAllObjects];
    for (NSNumber *pid in PI_enumerateProcesses()){
        kern_return = task_for_pid( mach_task_self(), [pid intValue], &task );
        if( kern_return == KERN_SUCCESS ) {
            NSString *name = [[pathFromPid([pid intValue]) componentsSeparatedByString:@"/"] lastObject];
            NSString *menuTitle = [name stringByAppendingFormat:@"(%lu)",[pid integerValue]];
            [_processSelector addItemWithTitle:menuTitle];
            [_PIDArray addObject:pid];
        }
    }
}

- (IBAction)processSelected:(id)sender {
    _selectedPID = [_PIDArray[[_processSelector indexOfSelectedItem]] intValue];
}

- (IBAction)selectFilePressed:(id)sender {
    [self showFileSelector];
}

- (IBAction)injectButtonPressed:(id)sender {
    printf("Injecting...\n");
    
    const char *path = [[_selectedFile path] UTF8String];
    printf("PID: %d\n",_selectedPID);
    printf("Path: %s\n",path);
    if (_selectedPID != 0 && path != NULL){
        inject(_selectedPID, path);
    }
}


- (void)showFileSelector {
    // Create the File Open Dialog class.
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    // Enable the selection of files in the dialog.
    [openDlg setCanChooseFiles:YES];
    
    // Multiple files not allowed
    [openDlg setAllowsMultipleSelection:NO];
    
    // Can't select a directory
    [openDlg setCanChooseDirectories:NO];
    
    // Display the dialog. If the OK button was pressed,
    // process the files.
    if ( [openDlg runModal] == NSModalResponseOK )
    {
        // Get an array containing the full filenames of all
        // files and directories selected.
        NSArray* urls = [openDlg URLs];
        
        // Loop through all the files and process them.
        for(int i = 0; i < [urls count]; i++ ){
            NSURL* url = [urls objectAtIndex:i];
            _selectedFile = url;
            [_selectedFileText setStringValue:[url lastPathComponent]];
            [_selectedFileText invalidateIntrinsicContentSize];
        }
    }
}

@end
