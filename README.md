# Vaultware

## Compilation

La compilation se fait avec Visual Studio. Les projets ont été créés avec la version 2022, 
il est cependant de changer le toolkit utilisé dans les propriétés de projets.

### Dépendances

- [wxWidgets](https://github.com/wxWidgets/wxWidgets): doit être installé sur le système, et la variable d'environement WXWIN doit pointer vers le dossier d'installation.
- [cpp-sqlite3](https://github.com/EddieBreeg/cpp-sqlite3)
- [Botan 2](https://github.com/randombit/botan)

A des fins de simplicités, les binaires sont disponibles sur la page [release](https://github.com/EddieBreeg/vaultware/releases). Le fichier lib.7z doit être décompressé à la racine du projet.