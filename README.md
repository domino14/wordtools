Some generic word tools. Requires Qt to build (latest version is best). Check downloads section for pre-packaged binaries.
Downloads section also has a set of files needed to run these scripts (.trie file for words and pre-compiled text files
with live/dead racks, etc.)

If you have any submissions to make these scripts faster, please go ahead!

Besides generic tools such as "build" and "anagram":

```
./wordtools anagram aeinrst
stainer
retains
stearin
ratines
antsier
retsina
anestri
nastier
retinas
9 total words 
```

```
./wordtools build cfiiivv
Building "cfiiivv" 
if
vivific
2 total words 
```

 we also have "revanag", suggested by Kenji. This can be thought of as a reverse anamonic finder for 6 and 7 letter racks. Use as follows:

wordtools revanag "AEIOU" 6 -- will find all 6 letter racks that make a 7-letter word with AEIOU (and possibly more letters).
Will add more tools in the future.
