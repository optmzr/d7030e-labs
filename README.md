# ψ D̵̗̳̱͠ȧ̸͎̿̔͝ͅ ̸̬̂̆̒͌̒b̸̼̆o̶̮̣̮͉͛̈́͝m̴̱͆̉̄͊̚ḃ̶̭̮̹͚͈͋͝͠ä̷̖͙̣d̴̼͎̿ͅ ̴̡̧̻̭̪͒̆̉̚D̵̢̞̈́̽͂7̴͉̼͈͍̓0̷̼̥̗̈̎̉͐͝3̸̼̽̇0̵͓̫̋̾̈E̵̪̲͓͕͑͝ ̸̛̲̣̍̽ḷ̵̻̖͎̣̾́a̶̢̺͔̳͌̒̂͗̚b̷͔͆́̋͐ȯ̵̩͇͕͘͠r̵̨̩̋͑̂͋ą̷̳͈͓̭͗͌̆̈̑ṭ̸̜̪̿̓͜e̵̲̥̫͝o̶̢̰̤̎͛n̴͙̈̒ ̵̜̣̏̿̋̀͝n̵͎͓̣̑̍̏͝ù̷̙̕͜m̷͈͉̺̞̋̉̉̔͘ͅb̸̺̝̯͙̠̐̉ȇ̴̟̤̗̪ŗ̴̣͓͐e̷͓͖̭̪͋̐͊͑ņ̸̪͕̊͒̾͆͝ ̷̥̮̩̦̪̔͛̏͠d̶͎̮̤͍͎̀́͂͘͝u̴̧̩̤̱̓e̵͔̘͈͚͎̎̿y̵͖͌̂ ⛥
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
~/d7030e-lab2 $ docker run \
		-v $(pwd)/scratch:/usr/ns3/scratch \
		-v $(pwd)/results:/usr/ns3/results \
		-it latest-ns3
```

### Usin' tha `sh -it`
Afta yo' entered da containa yo', build yo' scenario wit':
```
/usr/ns3 £ ./waf --run scenario1
```

Yo' can modify tha code inside tha `scratch/` folda directly outside tha docka containa:
```
~/d7030e-lab2 $ vi scratch/scenario1.cc
```

N' then recompile n' run wit':
```
/usr/ns3 £ ./waf --run scenario1
```

Afta yo' have run all 'em simulations yo' can peek inside da `results/` folda on yo' goddamn local host:
```
~/d7030e-lab2 $ ls -l results/
```
