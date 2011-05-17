#!/bin/bash


DBname=ledendatabase
ROname=ldb_ro
ROpass=verandermij
RWname=ldb_rw
RWpass=verandermijook

File=/tmp/ldb.sql.gz


echo "Maak een mooie nieuwe ledendatabase."
read -s -p "MySQL-wachtwoord voor root: " Rootpw
echo ""
echo ""

read -p "Database-naam [$DBname]: " tmp
if [ "$tmp" != "" ]; then DBname=$tmp; fi
echo ""

read -p "Readonly-account [$ROname]: " tmp
if [ "$tmp" != "" ]; then ROname=$tmp; fi
read -s -p "Readonly-wachtwoord: " ROpass
echo ""
echo ""
read -p "Read-write-account [$RWname]: " tmp
if [ "$tmp" != "" ]; then RWname=$tmp; fi
read -s -p "Read-write-wachtwoord: " RWpass
echo ""
echo ""

read -p "Backupbestand [$File]: " tmp
if [ "$tmp" != "" ]; then File=$tmp; fi
echo ""
echo ""


echo "Database maken... "
mysql -uroot -p$Rootpw -e "CREATE DATABASE $DBname;"
echo "klaar."
echo ""

echo "Login-accounts... "
mysql -uroot -p$Rootpw -e "CREATE USER '$ROname'@'localhost' IDENTIFIED BY '$ROpass'; \
    CREATE USER '$RWname'@'localhost' IDENTIFIED BY '$RWpass'; \
    GRANT SELECT ON $DBname.* TO '$ROname'@'localhost'; \
    GRANT SELECT,INSERT,UPDATE,DELETE ON $DBname.* TO '$RWname'@'localhost'; \
    "

echo "Klaar."
echo ""

echo "Backup terugzetten... "
gunzip $File
mysql -uroot -p$Rootpw $DBname < "${File%.gz}"
echo ""
echo "Klaar. Veel plezier met je nieuwe database."




