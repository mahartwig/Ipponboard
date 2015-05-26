# *Ipponboard* Read-Me #

## Legal Notice ##

## Changes ##

### Version 1.4 (team edition valid until 01/2015) ###
  - (new): tournament modes can be configured nicely within the program (Management -> Modes)
  - (mod): removed menu entry to set the round time in team edition as it was confusing with the new mode management
  - (mod): changed the type of the internal label of the tournament modes from string to an unique identifier
  - (fix): fight time for women leagues was 5 instead of 4 minutes 
  - (fix): changed encoding of tournament modes configuration file to UTF-8
  - (fix): tournament modes were not sorted alphabetically
  - (fix): layout of remaining time was not proper in list templates
  - (fix): translation (DE) was missing in some parts of the basic edition
  - (fix): removed empty Open/Save command from menu

### Version 1.3.3 (team edition valid until 01/2015)
  - (mod): (!) deactivated fighter management for team edition - feature not 100% convincing (yet)
  - (fix): rules and other options were not properly propagated to fights
  - (fix): view was not updated in some cases

### Version 1.3.beta (team edition valid until 01/2014)
  - (new): whole new fighter management
	- import/export
	- selection of fighters via dropdown menu
	- all fighters are saved
  - (new): configurable tournament modes
	- up to two rounds
	- unlimited fights per round
	- different print/export templates
	- different set of weight classes
	- different fight times
  - (new): redesigned some parts of the UI
	- re-worked visualization of saved fights

### Version 1.2.2 (team edition valid until 01/2014)
  - (mod): all leagues use the 2013 IJF rules per default
  - (mod): reduced expiration countdown time to 7 seconds (from 15)
  - (fix): updated to Qt 4.8.5 and VC12, still supporting Windows XP
	
### Version 1.2.1
  - (mod): all primer leagues use IJF rules of 2013 per default all others don't
  - (fix): single shidos did lead to a wrong result in the score and subscore (2013 rules only, issue #27)
	
### Version 1.2
  - (new): implemented new IJF rules of 2013
    - Penalties(1): Shidos do not give points to the other fighter (except forth Shido = Hasokumake). At the end of the fight, if scoring is equal, the one with less Shido wins. (issue #18)
    - Osaekomi: 10 seconds for Yuko, 15 seconds for Waza-ari, and 20 seconds for Ippon (issue #20)
	- No time limit for Golden Score (Hantai is cancelled) (issue #19)
  - (new): automatic setting of points within Osaekomi or due to penalties can be turned off in the program settings (issue #14)
  - (new): default weight classes match new DJB specification (issue #16)
  - (new): support for women leagues (issue #7, team edition only)
  - (new): made labels for "home" and "guest" editable in settings page (team edition only)
  - (new): Windows 8 support
  - (mod): replaced the hourglass in Golden Score mode with a golden text (basic edition only)
  - (mod): the score values can be edited in table (fixes issue #10, team edition only)
  - (mod): the detected screen dimensions are shown in the settings dialog
  - (fix): IJF conformity: changed initial sides (blue is now on the left side, white on the right) (fixes issue #17)
  - (fix): Hold is not aborted in golden score after scoring first yuko (issue #2)
  - (fix): checkbox for automatically detecting screen size did not enable dimension input fields for manual setting when unchecked
  - (fix): don't allow invalid times like "-1:22" to be set for main timer

### Version 1.0.1
  - removed time limitation

### Version 1.0
- (new): optimized GUI
- (new): separate osaekomi controls for each fighter (this greatly improves mouse handling!)
- (new): separate font configuration for the fighter names
- (new): separate mat signal button
- (new): the mat label can now be configured freely
- (mod): changed some text labels
- (mod): improved readability: score labels are bigger now
- (mod): redesigned settings dialog (main settings and gamepad controls)
- (mod): reordered control elements on main window
- (fix): some internal glitches
- (fix): reduced average cpu load from 10% to less than 1%
- (fix): the hold color was not properly reset after a reset
- (fix): fixed some portable installation issues

### Until 2011-01-03:
- (new): fight categories are now fully editable (allowing adding/removing/(mod):ifying/renaming)
- (new): information header (mat number, category, weight class) can now be switched on and off
- (new): the setup now provides a clean portable installation type
- (new): program icons: now using Fugue’s incredible and splendid icon set to freshen up the GUI
- (fix): some minor code fixes

### Until 2010-10-11:
- (new): The splashscreen button is now focused by default
- (new): The program language can now be set in the main menu (available: English and German)
- (new): Added some explaining tooltips to the settings dialog
- (mod): The weight extension “kg” is not shown anymore
- (mod): Output file name now has no spaces anymore
- (fix): Sometimes the UI got stuck
- (fix): Some minor textual corrections in the German manual
- (fix): The text preview now instantly responds to font style changes (bold, italic)
- (fix): End year of the trial period is 2011 not 2010…
- (fix): Corrected version information in setup file