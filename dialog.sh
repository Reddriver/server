#!/bin/bash

vytvorAdresarNAS() {
        sudo mkdir $CESTA
        sudo chown $JMENO:$JMENO $CESTA
        sudo chmod 0777 $CESTA
}

strukturaMenu() {
        #vytvoreni adresare pro NAS
        if [ $exitstatus = 0 ]; then
                if [ $OPTION = 1 ]; then
                        JMENO=$(whiptail --title "Vlastnik NAS" --inputbox "Zadejte jmeno uzivatele" 10 60 luka 3>&1 1>&2 2>&3)
                        exitstatus=$?
                        if [ $exitstatus = 0 ]; then
                                CESTA=$(whiptail --title "Cesta k NAS" --inputbox "Zadejte cestu k adresari NAS" 10 60 /adresar 3>&1 1>&2 2>&3)
                                exitstatus=$?
                                if [ $exitstatus = 0 ]; then
                                        if [ -d $CESTA ]; then
                                                if ( whiptail --title "Smazat obsah adresare" --yesno "Adresar jiz existuje, chcete vymazat obsah?" 10 60 ); then
                                                        pswd=$(whiptail --title "Zadani heslo root" --passwordbox "Zadejte heslo root" 10 60 3>&1 1>&2 2>&3)
							exitstatus=$?
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
                fi
		menu
        fi
}

menu() {
	OPTION=$(whiptail --title "Instalace serveru" --menu "Vyberte z nabídky" 15 60 4 \
	"1" "Vytvoreni adresare pro NAS" \
	"2" "Instalace PYLOAD" 3>&1 1>&2 2>&3)
	exitstatus=$?
	strukturaMenu
}

menu
