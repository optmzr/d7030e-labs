# ψ D̵̗͎̘̭̀̊̅̎å̴͝ͅ ̶̛̥b̶̥̯̠̹͗̏o̶̯͕̦̘͌̈́͒͆̚m̷̛͚̮̑̍͘b̸̮͕̬̰̭̈́̓͛á̸̛͓̘̪̍͝d̸͙̘̺̞̣͑́̆ ̴̗̼͖̥͊d̶̮̉̇͋͆̕7̴̡̬̈́͂̌̚ͅ0̶̭͓̏̄̅3̷͎̖͕̻̗̀̃̑0̸̟̒̔e̵̝̪͛͂́̇ ̶̠̪̬̩̅̏͒́͊l̷̳̘̞͚̄̚͠͠͠a̵̧̛̜̠̝͂̒͘͝b̶̧̫̺̻̈́͝ͅò̷̡͍̖̖̩̓͗͝r̸̙̔a̴̝̳͒̅̈́̓ͅt̵̬͔̟͉̗̐͒͊e̷̦̞͉̿̿͜ͅö̶̭́͗̿̚ͅn̶͉͝ ̶̘͎͈̊̿̓̚n̶͓̯̼̹̈̌̅̋u̸̠̓͌͝m̵̖̏̀͒͝b̶̧͉͇͖̕͜ȩ̸̡̗̻̹̀̌́ṟ̶̰̼̙̺͐̾͌͆ė̸̳̒͐̈́n̶̛͔̣̩̞͗̃ ̷̲̗̑͑͆͂̚d̷̢̡̰̂̈͘u̸͍̳͊̽͒͝e̷̝̰̯̜̓y̶̝̟̱̑ ⛥
### Understandin' tha symbols
 * `$` - Yo' local hood, dawg.
 * `£` - Inside tha containa, thug.

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
/usr/ns3 £ ./waf
```

### Yo' can modify tha code inside tha `/src` folda directly outside tha docka containa:
```
~/d7030e-lab2 $ vi src/scenario1.cpp
```

N' then recompile n' run wit':
```
/usr/ns3 £ ./waf
```
