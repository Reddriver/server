#!/bin/sh
echo "Zahajuji instalaci..."
echo "Nastavit cestu k NAS?"

#vytvoreni adresare pro NAS
read volba
if [ $volba = "y" ]; then
	echo Zadejte jmeno uzivatele:
	read jmeno
	echo Zadejte cestu kde bude NAS:
	read cesta
	#hardcoded:
	#jmeno="luka"
	#cesta="/aaa"
	if [ -d $cesta ]; then
		echo "Adresar pro NAS jiz existuje."
	else
		sudo mkdir $cesta
		sudo chown $jmeno:$jmeno $cesta
		sudo chmod 0777 $cesta
		echo "Adresar pro NAS je vytvořen v" $cesta "a nastaven na uzivatele" $jmeno "s pristupovymi pravy 0777."
	fi
fi

#instalace pyload
echo "Chcete nyni nainstalovat PyLoad?"
read volba
if [ $volba = "y" ]; then
	cesta="/home/luka"
	cd $cesta
	mkdir pyload2
	git clone -b cs https://github.com/OndrejIT/pyload.git
	sudo mkdir /pyload2 
	cd pyload
	echo "Prvni spusteni instalace pyload - zmenit cestu k nastaveni na" $cesta
	sudo python pyloadCore -s
	echo "Druhe spusteni instalace pyload."
	sudo cp myscript.service /lib/systemd/system
	sudo systemctl daemon-reload
	sudo systemctl enable myscript.service
	echo "Instalace PyLoad dokoncena. Po restartu zmenit prava na webu PyLoad - nogroup:nobody a 0777."
fi
echo "Instalace dokoncena..."
