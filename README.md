# L-Système

## Compilation

Pour générer toutes les cibles :

```bash
make lsystem trace
```

Dépendances :

- La bibliothèque [Allegro5](https://liballeg.org/) pour les graphiques.
- La bibliothèque [bnf](https://github.com/Juroupi/bnf) pour les grammaires.

## Fichiers générés

- `lsystem` : le programme pour générer et afficher des L-Systèmes à partir de fichiers `.bnf`.
- `trace` : le programme pour générer une moyenne des images qui peuvent être générées par `lsystem`.

## Paramètres

Les paramètres de `lsystem` doivent être passés sous la forme `<param>=<valeur>` dans un ordre quelconque. Les paramètres possibles sont les suivants :

- `file` : le fichier à tester (obligatoire).
- `start` : le symbole non terminal à dériver (obligatoire).
- `angle` : l'angle de variation en degrés (obligatoire).
- `card` : la cardinalité de la chaîne de caractère à générer (obligatoire).
- `method` : la méthode de génération `uniform` ou `nuniform`.
- `x` et `y` : la position initiale du curseur.
- `a` : l'orientation initiale du curseur en degrés.
- `d` : la longueur initiale des traits.
- `width` et `height` : la taille de la fenêtre.

## Exemples

Il y a plusieurs exemples dans le dossier `tests` qui peuvent être directement exécutés avec `make` (voir dans le Makefile), par exemple `make tree` qui va exécuter la commande `./lsystem file=tests/tree.bnf start=T angle=30 card=601 y=350 a=-90`. Il suffit d'appuyer sur la touche espace pour générer un nouveau dessin aléatoire dans `lsystem`.
