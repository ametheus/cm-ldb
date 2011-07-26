#!/bin/bash

#   
#   Copyright (C) 2011 Thijs van Dijk
#   
#   This file is part of CM-LDB.
#   
#   CM-LDB is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   CM-LDB is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with CM-LDB.  If not, see <http://www.gnu.org/licenses/>.
#   

#   
#   Create a new database on the current host.
#   Note: requires a working backup of the last one.
#   



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




