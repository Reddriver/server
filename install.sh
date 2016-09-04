#!/bin/sh
echo "Zahajuji instalaci..."
echo "Nastavit cestu k NAS?"
read volba
if [ $volba = "y" ]; then
	echo Zadejte jmeno uzivatele:
	#read jmeno
	echo Zadejte cestu kde bude NAS:
	#read cesta
	#hardcoded:
	jmeno="luka"
	cesta="/aaa"
	if [ -d $cesta ]; then
		echo "Adresar pro NAS jiz existuje."
	else
		sudo mkdir $cesta
		sudo chown $jmeno:$jmeno $cesta
		sudo chmod 0777 $cesta
		echo "Adresar pro NAS je vytvořen v" $cesta "a nastaven na uzivatele" $jmeno "s pristupovymi pravy 0777"
	fi
fi
echo "Instalace dokoncena..."
