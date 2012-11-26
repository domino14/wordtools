Some generic word tools. Requires Qt to build (latest version is best). Check downloads section for pre-packaged binaries.

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

wordtools revanag "AEIOU" 6 -- will find all 6 letter racks that make a 7-letter word with AEIOU (and possibly more letters). Warning - this is very slow right now. A way to speed it up would be to modify the generateLiveRacks function to output all tiles that go with rack and then just do a text lookup. This shouldn't be too hard and I'll do it when I have time :P

Will add more tools in the future.