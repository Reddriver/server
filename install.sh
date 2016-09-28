#!/bin/sh
echo "Zahajuji instalaci..."
#echo "Nastavit cestu k NAS?"

#vytvoreni adresare pro NAS
#read volba
#if [ $volba = "y" ]; then
#	echo Zadejte jmeno uzivatele:
#	read jmeno
#	echo Zadejte cestu kde bude NAS:
#	read cesta
	#hardcoded:
	#jmeno="luka"
	#cesta="/aaa"
#	if [ -d $cesta ]; then
#		echo "Adresar pro NAS jiz existuje."
#	else
#		sudo mkdir $cesta
#		sudo chown $jmeno:$jmeno $cesta
#		sudo chmod 0777 $cesta
#		echo "Adresar pro NAS je vytvořen v" $cesta "a nastaven na uzivatele" $jmeno "s pristupovymi pravy 0777."
#	fi
#fi

#instalace pyload
echo "Chcete nyni nainstalovat PyLoad?"
read volba
if [ $volba = "y" ]; then
	echo "Prejete si zadat cestu pro pyload?"
	read volba2
	if [ $volba2 = "y" ]; then
		echo "Zadejte cestu pro pyload"
		read cestaPyload
	else
		cestaPyload="/home/luka/pyload"
	fi

	if [ ! -d "$cestaPyload" ]; then
		mkdir $cestaPyload
	else
		echo "Zadana cesta ( $cestaPyload ) jiz existuje. Chcete obsah adresare odstranit?"
		read volba3
		if [ $volba3 = "y" ]; then
			rm -rf $cestaPyload
			mkdir $cestaPyload
		fi
	fi
	pwd
	cd $cestaPyload
	echo "Prejete si oklonovat z github do" $cestaPyload"?"
	read volba4
	if [ $volba4 = "y" ]; then
		git clone -b cs https://github.com/OndrejIT/pyload.git
	fi
	echo "Prejete si vytvorit slozku nastaveni /pyload ?"
	read volba5
	if [ $volba5 = "y" ]; then
		echo "Zadejte cestu k nastaveni (defaultni je /pyload)"
		read cestaNastaveni
	else
		cestaNastaveni="/pyload"
	fi
	if [ ! -d "$cestaNastaveni" ]; then
                sudo mkdir $cestaNastaveni
        else
                echo "Zadana cesta "$cestaNastaveni" jiz existuje. Chcete obsah adresare odstranit?"
                read volba6
                if [ $volba6 = "y" ]; then
                        sudo rm -rf $cestaNastaveni
                        sudo mkdir $cestaNastaveni
                fi
        fi

	cd pyload
	echo "Nyni bude nastalován pycurl a rhino"
	sudo apt-get update
	sudo apt-get install python-pycurl
	sudo apt-get install rhino
	echo "Prvni spusteni instalace pyload - zmenit cestu k nastaveni na" $cestaNastaveni
	sudo python pyLoadCore.py -s
	echo "Druhe spusteni instalace pyload."
	sudo python pyLoadCore.py -s

sudo cat >/lib/systemd/system/pyload.service <<EOF
[Unit]
Description=Pyload deamon
After=network.target
[Service]
ExecStart=/luka/home/pyload/pyload/pyLoadCore.py
User=root
Group=root

[Install]
WantedBy=multi-user.target
EOF

	sudo systemctl daemon-reload
	sudo systemctl enable pyload.service
	echo "Overeni ze je sluzba vytvorena"
	ls /lib/systemd/system | grep pyload
	echo "Instalace PyLoad dokoncena. Po restartu zmenit prava na webu PyLoad - nogroup:nobody a 0777."
fi
echo "Instalace dokoncena..."
