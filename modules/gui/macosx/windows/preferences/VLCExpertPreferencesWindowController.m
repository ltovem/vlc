//
//  VLCExpertPreferencesWindowController.m
//  Pseudo-VLC
//
//  Created by Marvin Scholz on 12.01.23.
//

#import <vlc_common.h>
#import <vlc_plugin.h>

#import "VLCExpertPreferencesWindowController.h"
#import "coreinteraction/VLCModule.h"
#import "coreinteraction/VLCModuleConfigItem.h"

@interface VLCExpertPreferencesWindowController<NSTableViewDelegate> ()

@property (weak) IBOutlet NSArrayController *arrayController;
@property (unsafe_unretained) IBOutlet NSTableView *preferencesTable;

@property (strong) NSMutableArray<VLCModuleConfigItem *> *configItems;

@end

@implementation VLCExpertPreferencesWindowController

- (NSNibName)windowNibName
{
    return @"ExpertPreferencesWindow";
}

- (void)windowDidLoad
{
    [super windowDidLoad];

    // To mark the doc as change, do:
    // self.window.documentEdited = YES;
    // Put initialization code here that runs after the Window loaded
    _configItems = [NSMutableArray arrayWithCapacity:1600];

    [self willChangeValueForKey:@"configItems"];
    for (VLCModule *mod in [VLCModule moduleList]) {
        for (VLCModuleConfigItem *configItem in mod.configItems) {
            if (configItem.isHint)
                continue;
            if (configItem.type == CONFIG_ITEM_KEY) {
                // FIXME: Handle keys
                // While these are skipped in the Qt interface too currently,
                // I do think we should handle them for consistency, so that
                // the expert preferences really contain all options.
                continue;
            }
            [self insertConfigItemSorted:configItem];
        }
    }
    [self didChangeValueForKey:@"configItems"];

    // Set initial sort to the sort descriptor of the first column
    // This has the side-effect of reloading the table data.
    NSSortDescriptor *firstColSort = _preferencesTable.tableColumns.firstObject.sortDescriptorPrototype;
    [_preferencesTable setSortDescriptors:@[firstColSort]];
}

- (void)insertConfigItemSorted:(VLCModuleConfigItem *)item
{
    NSUInteger insertionIndex = [_configItems indexOfObject:item
                                              inSortedRange:NSMakeRange(0, _configItems.count)
                                                    options:NSBinarySearchingInsertionIndex
                                            usingComparator:^NSComparisonResult(id  _Nonnull obj1, id  _Nonnull obj2) {
        VLCModuleConfigItem *config1 = obj1;
        VLCModuleConfigItem *config2 = obj2;
        return [config1.displayName compare:config2.displayName];
    }];
    [_configItems insertObject:item atIndex:insertionIndex];
}

#pragma mark - Table Delegate

- (NSView *)tableView:(NSTableView *)tableView
   viewForTableColumn:(NSTableColumn *)tableColumn
                  row:(NSInteger)row
{
    NSString *identifier = tableColumn.identifier;
    if ([identifier isEqualToString:@"valueColumn"]) {
        VLCModuleConfigItem *item = _arrayController.arrangedObjects[row];
        switch (item.type) {
            case CONFIG_ITEM_STRING:
            case CONFIG_ITEM_MODULE:
            case CONFIG_ITEM_MODULE_CAT:
            case CONFIG_ITEM_MODULE_LIST:
            case CONFIG_ITEM_MODULE_LIST_CAT:
            case CONFIG_ITEM_FONT:
                identifier = (item.choices) ? @"choiceValueCell" : @"stringValueCell";
                break;
            case CONFIG_ITEM_PASSWORD:
                identifier = @"passwordValueCell";
                break;
            case CONFIG_ITEM_BOOL:
                identifier = @"booleanValueCell";
                break;
            case CONFIG_ITEM_INTEGER:
                identifier = (item.choices) ? @"choiceValueCell" : @"integerValueCell";
                break;
            case CONFIG_ITEM_RGB:
                identifier = @"colorValueCell";
                break;
            case CONFIG_ITEM_FLOAT:
                identifier = @"floatValueCell";
                break;
            case CONFIG_ITEM_LOADFILE:
            case CONFIG_ITEM_SAVEFILE:
                identifier = @"fileValueCell";
                break;
            case CONFIG_ITEM_DIRECTORY:
                identifier = @"folderValueCell";
                break;
                
            default:
                break;
        }
    }
    NSTableCellView *cellView = [tableView makeViewWithIdentifier:identifier owner:self];
    return cellView;
}

#pragma mark - Action handlers

- (IBAction)saveSettings:(id)sender {
    [_configItems makeObjectsPerformSelector:@selector(apply)];
}

@end
