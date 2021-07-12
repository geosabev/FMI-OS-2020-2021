#!/bin/bash

[ $# -eq 2 ] || { echo "Two arguments are needed!"; exit 1; }

[[ "$1" =~ .+.csv$ ]] || { echo "First argument should be a valid .csv filename!"; exit 2; }

[ -d $2 ] || { echo "Second argument should be a directory!"; exit 3; }

touch $1

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > $1

while read filename; do
    
    hostname="$(echo $(basename $filename) | sed 's/.log//')"
    phy="$(cat $filename | head -n 2 | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' |cut -d ':' -f 2-)"
    vlans="$(cat $filename | head -n 3 | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"
    hosts="$(cat $filename | head -n 4 | tail -n 1 |sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"
    failover="$(cat $filename | head -n 5 | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"
    vpn="$(cat $filename | head -n 6 | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"
    peers="$(cat $filename | head -n 7 | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"
    ports="$(cat $filename | head -n 8 | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"
    license="$(cat $filename | grep "This platform has " | sed -E 's/This platform has a[n]? //' | sed -E 's/ license.//')"
    sn="$(cat $filename | head | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"
    key="$(cat $filename | head -n 11 | tail -n 1 | sed -E 's/[ ]*:[ ]*/:/' | cut -d ':' -f 2-)"

    echo $hostname,$phy,$vlans,$hosts,$failover,$vpn,$peers,$ports,$license,$sn,$key >> $1

done < <(find $2 | egrep ".log$")
