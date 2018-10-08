# ☠ Ţ̶͍̮̳͔̐̍h̴̡̲̲̯̓̌ë̵̪́ ̷̬̺̥̹͉̓͋̄̚ḁ̶̡̺̖̌͐͝͝w̵͓͉͉̺̪̽̆̒e̸̢̛̤͈̞̊̂̈́s̶̡̗͓͖͕̈̕͝o̷̹̙͛̂̑̓̐͜m̵͙͎̝̉ë̶̢̪̪̥͔́ ̸̥̘̮͊̈́͛̓͝ͅĎ̴͓͕̂͊̎̔7̴͈̫̐͜0̴̗͑̌̒3̷̭̯͑̽͜0̶̢̞̳̱̺̊͑͂̎̔Ȅ̸̮̌̒̕ͅͅ ̶̻̲̺̣͉̾̽̏̐l̵̡̛͉͙͍͆͘ǎ̷͈̎͌b̴̨̨̛̥͍̎ọ̵̪̙̾̊͋ŗ̷̖͇͙̪̍̒̅͒̕a̴̘̹͐̆͊̐t̴̹̼̼͛̏ì̷͍̗̤̽̿́̿o̸̫̟̓̔̿̊n̶͇͕̰̐̍̈́͠ ̷͈͋͛͠ṇ̵̠͑̎ư̴̦̯̓̽̓m̵̡̠̖͕̿̚b̷̲̠́̆̇ě̶̖̗̒̚ͅr̸̡̬̈ ̶̢̜̝͇̦͑̚d̸̫͕̂̇̚̚͠o̸̗̱͋ṡ̴̰ ⛥
### Understandin' tha symbols
`$` - Yo' local hood, dawg.
`£` - Inside tha containa, thug.

### Start da `sh -it`
So ya'll need to build da docker image with NS-3.
```
~/d7030e-lab2 $ docker build -t latest-ns3 .
```

### Enta da containa
Then yo' need ta git tha containa the fsck up n' runnin', n' enta dat homeboy!
```
~/d7030e-lab2 $ docker run -v src:/usr/ns3/scratch -it latest-ns3
```

### Usin' tha `sh -it`
Afta yo' entered da containa yo', build yo' images wit':
```
/usr/ns-3 £ ./waf
```

### Yo' can modify tha code inside tha `/src` folda directly outside tha docka containa:
```
~/d7030e-lab2 $ vi src/scenario1.cpp
```

N' then recompile n' run wit':
```
/usr/ns-3 £ ./waf
```
