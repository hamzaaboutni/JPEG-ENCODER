# Notre encodeur JPEG à nous
![Résumé](images/IMG_7388.jpg)

# Organisation du projet:

1. Mehdi se charge de : La lecture du fichier pgm/ppm , gestion des paramètres, découpage en MCU, conversion YCbCr et le sous-échantillonage.

- Structures de données chosies : un pixel est répresénté par un triplé de unsigned char, rouge, vert, bleu. une MCU est représentée par une matrice de pixels
et une MCU sous-échantillonée est représentée par des blocks int**. On a choisi de représenter les matrices par des doubles pointeurs pour des raisons de performances.
- On a choisi de découper l'image en une matrice de MCUs puis ensuite, aplatir cette matrice pour obtenir une liste de mcus sur laquelle on va appliquer la DCT, QQ+ZZ.
- Le subsampling (ou downsampling) prend en charge tous les facteurs (hi,vi) possibles, tant qu'il sont conformes aux conditions données dans l'énoncé.
- Le programme est robuste contre les entrées invalides de l'utilisateur par ex : des facteurs de sous-échantillonage invalides, fichier inexistant, format non pris en charge...

## DCT & Q+ZZ : 

#### Hamza se charge de la Transformée en cosinus discrète et de la **quantification** et **zig-zag** ;

>**04/05/2024** : La DCT est écrite et testée en utilisant plusieurs types de blocs : **uniforme**(i.e un bloc dont toutes les valeurs sont les mêmes ), **Blocs diversifié**( pour voir comment la DCT manipule les fréquences variées) ainsi qu'une **vérification de la symétrie** par un bloc avec des motifs symétriques (pour pour vérifier si la DCT reflète correctement la symétrie dans le domaine fréquentiel.) 

### Spécifications : 
- On l'a déclarée dans [dct.h](./include/dct.h) et définie dans [dct.c](./src/dct.c) et porte le nom **dct** prend en paramètres un bloc d'entrée double block[N][N] et un bloc de sortie double dctBlock[N][N] et réalise le calcul sur un seul bloc .

- La mémoire doit être allouée et libérées à l'extérieur de la fct 'dct' , cela permet de réutiliser la mémoie et d'éviter des allocations et des désallocations fréquentes , ce qui est couteux en termes de performances. 

- On utilise un tableau 2D de taille 8x8 (double block[8][8]) pour représenter un bloc de données dans le contexte de la DCT. Cela correspond aux dimensions naturelles d'un bloc dans l'encodage JPEG et facilite les opérations sur le bloc.
> **Nota Bene :** On est concsient que ce premier programme est de complexité quadratique O($n^2$) et l'on peut améliorer en utilisant des méthodes de type "Diviser pour régner" pour écrire une version efficace de la transformée de Fourier (Fast Fourier Transform, FFT) afin de réduire la complexité à O(N $\log(N)$) .Ces implémentations tirent parti de la structure et des symétries de la DCT , ce qui est particulièrement avantageux pour des tailles de bloc plus grandes.

### Q+ZZ  :
>**05/05/2024** : 
L'opération de réorganisation en "zig-zag" permet de représenter un bloc **$8*8$** sous forme de vecteur 1D de 64 coefficients.A l'encodage, la quantification consiste à diviser terme à terme chaque bloc **$8x8$** par une matrice de quantification, elle aussi de taille **$8x8$**. Les résultats sont arrondis, de sorte que plusieurs coefficients initialement différents ont la même valeur après quantification.

## Huffman :
#### Mohammed Amine s'occupe de l'écriture bit à bit dans le bitstream , et de l'encodage Huffman
>**06/05/2024** : Dans le cadre d'un projet JPEG, j'ai pris en charge la fonction Huffman, une des fonctions les plus complexes à implémenter. Comme vous le savez, cette tâche est réputée pour sa difficulté. J'ai donc commencé par suivre l'énoncé étape par étape, ligne par ligne, afin de créer un code naïf mais fonctionnel. Initialement, j'ai mis en place des fonctions basiques permettant de générer le code d'un symbole à partir des tables de Huffman (htables), ainsi que des fonctions pour calculer la magnitude et l'indice dans la classe de magnitude. Ensuite, j'ai progressivement ajusté le codage des coefficients DC, puis AC, et enfin pour chaque bloc de 8x8 pixels. Pour couronner le tout, j'ai développé une fonction générale appelée Huffman, capable de prendre en entrée une grande liste de blocs de différents types et de produire un flux complet de l'image codée.

**13/05/2024** : Depuis ce jour-là, j'ai entamé une toute nouvelle aventure d'optimisation de la fonction Huffman. Le programme initial, bien que fonctionnel, était très complexe en raison des nombreuses allocations de mémoire et des répétitions. Pour remédier à cela, j'ai dû travailler en complément du type char avec une autre structure de tampon dynamique. Cette nouvelle structure a grandement facilité la gestion de la mémoire et des opérations répétitives, telles que la concaténation. Grâce à cette optimisation, le code est devenu plus efficace et plus facile à maintenir.


# Liens utiles

- Bien former ses messages de commits : [https://www.conventionalcommits.org/en/v1.0.0/](https://www.conventionalcommits.org/en/v1.0.0/) ;
- Problème relationnel au sein du groupe ? Contactez [Pascal](https://fr.wikipedia.org/wiki/Pascal,_le_grand_fr%C3%A8re) !
- Besoin de prendre l'air ? Le [Mont Rachais](https://fr.wikipedia.org/wiki/Mont_Rachais) est accessible à pieds depuis la salle E301 !
- Un peu juste sur le projet à quelques heures de la deadline ? Le [Montrachet](https://www.vinatis.com/achat-vin-puligny-montrachet) peut faire passer l'envie à vos profs de vous mettre une tôle !