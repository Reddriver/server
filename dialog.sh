#!/bin/bash

vytvorAdresarNAS() {
        sudo mkdir $CESTA
        sudo chown $JMENO:$JMENO $CESTA
        sudo chmod 0777 $CESTA
}

zadaniHeslaRoot() {
	 pswd=$(whiptail --title "Zadani heslo root" --passwordbox "Zadejte heslo root" 10 60 3>&1 1>&2 2>&3)
         exitstatus=$?
}

getStatusSudo() {
	#test sudo opravneni
	CAN_I_RUN_SUDO=$(sudo -n uptime 2>&1|grep "load"|wc -l)
	if [ ${CAN_I_RUN_SUDO} = 0 ]; then
		zadaniHeslaRoot()
	fi
}

restartServeru() {
	getStatusSudo
	sudo reboot
}

vytvorNAS(){
	JMENO=$(whiptail --title "Vlastnik NAS" --inputbox "Zadejte jmeno uzivatele" 10 60 luka 3>&1 1>&2 2>&3)
        exitstatus=$?
        if [ $exitstatus = 0 ]; then
        	CESTA=$(whiptail --title "Cesta k NAS" --inputbox "Zadejte cestu k adresari NAS" 10 60 /adresar 3>&1 1>&2 2>&3)
                exitstatus=$?
                if [ $exitstatus = 0 ]; then
	                if [ -d $CESTA ]; then
        	                if ( whiptail --title "Smazat obsah adresare" --yesno "Adresar jiz existuje, chcete vymazat obsah?" 10 60 ); then
        	                      	getStatusSudo
                                        if [ $exitstatus = 0 ]; then
                 	                       sudo -S <<< $pswd rm -rf $CESTA
                                               vytvorAdresarNAS
                                               whiptail --title "Adresar NAS" --msgbox "Obsah adresare vymazan!" 10 60
                                        else
                                               whiptail --title "Heslo root" --msgbox "Heslo bylo zadano spatne!" 10 60
                                        fi
                                fi
                        else
	               	        vytvorAdresarNAS
                                text="Adresar pro NAS je vytvořen v $CESTA a nastaven na uzivatele $JMENO s pristupovymi pravy 0777."
                                whiptail --title "Adresar NAS" --msgbox "$text" 10 100
                       fi
                fi
        fi
}

aktualizujKernel() {
	cd /tmp
	if ! which lynx > /dev/null; then sudo apt-get install lynx -y; fi
	if [ "$(getconf LONG_BIT)" == "64" ]; then arch=amd64; else arch=i386; fi

	function download() {
	   wget $(lynx -dump -listonly -dont-wrap-pre $kernelURL | grep "$1" | grep "$2" | grep "$arch" | cut -d ' ' -f 4)
	}

	# Kernel URL
	kernelURL=$(lynx -dump -nonumbers http://kernel.ubuntu.com/~kernel-ppa/mainline/ | tail -1)
	echo "Stahuji posledni kernel kernel."
   	download generic header
   	download generic image
	wget $(lynx -dump -listonly -dont-wrap-pre $kernelURL | grep all | cut -d ' ' -f 4)
	# Install Kernel
	echo "Instaluji Linux Kernel"
	getStatusSudo
	if [ $exitstatus = 0 ]; then
		sudo -S <<< $pswd dpkg -i linux*.deb
	fi
	whiptail --title "Aktulizace kernelu" --msgbox "Novy kernel nainstalovan, je potreba restart." 10 100
}

strukturaMenu() {
	case $OPTION in
	1) vytvorNAS ;;
	3) aktualizujKernel ;;
	4) restartServeru ;;
	*) echo "Neplatná známka" ;;
	esac
	menu     
}

menu() {
	OPTION=$(whiptail --title "Instalace serveru" --menu "Vyberte z nabídky" 15 60 4 \
	"1" "Vytvoreni adresare pro NAS" \
	"2" "Instalace PYLOAD" \
	"3" "Aktualizace kernelu" \
	"4" "Restart serveru" \
	3>&1 1>&2 2>&3)
	exitstatus=$?
	if [ $exitstatus = 0 ]; then
		strukturaMenu
	fi
}

menu
