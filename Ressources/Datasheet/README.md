# Datasheets — documentation des composants actifs

Une entrée par circuit intégré du schéma, nommée par sa **référence dans le schéma**
(`U1`…`U9`), pour que le lien PDF ↔ feuille KiCad soit immédiat.

## Comment s'en servir

| | |
|---|---|
| `*.pdf` | **La source de vérité.** Contient les schémas, courbes et figures. |
| `txt/*.txt` | Texte intégral extrait, **une balise `===== page N =====` par page**. Le numéro correspond à la page du PDF. C'est la version cherchable. |
| `extract_text.py` | Régénère `txt/` (`python extract_text.py` ou `python extract_text.py U5`). Dépend de `pypdf`. |

Chercher dans toutes les datasheets à la fois :

```bash
grep -rn "bootstrap" "Ressources/Datasheet/txt/"
```

Chercher dans une seule et retrouver la page :

```bash
grep -n "GVDD" "Ressources/Datasheet/txt/U5_TPA3255.txt" | head
```

L'extraction texte **perd les figures, les schémas d'application et les courbes**, et
aplatit les tableaux à colonnes multiples. Pour un brochage, une courbe de rendement
ou un schéma type, ouvrir le PDF à la page indiquée ci-dessous.

## Inventaire

| Réf. | Composant | Rôle | Feuille | Boîtier | Document | p. |
|---|---|---|---|---|---|---|
| U1 | FSC-BT1058 | Module Bluetooth 5.3 dual mode + LE Audio (Qualcomm) | `bluetooth` | module SMD | Feasycom, V1.0 (2024) | 41 |
| U2 | PCM1863DBT | ADC stéréo 110 dB, PGA + I²S | `wired_input` | TSSOP-30 | TI **SLAS831D** (mars 2018) | 145 |
| U3 | PCM5242RHBT | DAC stéréo 114 dB avec miniDSP | `eq_and_dac` | VQFN-32 | TI **SLASE12A** (oct. 2014) | 129 |
| U4 | TMUX1574PW | Mux/démux 4:1 ×1 — aiguillage I²S | `eq_and_dac` | TSSOP-16 | TI **SCDS391C** (déc. 2019) | 45 |
| U5 | TPA3255DDV | Ampli classe D 2×315 W, PVDD ≤ 53,5 V | `amplification` | HTSSOP-44 PowerPAD | TI **SLASEA8A** (oct. 2016) | 47 |
| U6 | TLV76733DRVx | LDO 1 A / 16 V → **+3.3V** numérique | `alimentation` | WSON-6 | TI **SLVSE84D** (juil. 2021) | 44 |
| U7 | LM5017MR | Buck synchrone 100 V / 600 mA → **+12V** | `alimentation` | SO PowerPAD-8 | TI **SNVS783K** (août 2021) | 41 |
| U8 | LT3042xMSE | LDO ultra-faible bruit 200 mA → **+3.3VA** | `alimentation` | MSOP-10-EP | ADI **3042fa** | 30 |
| U9 | STM32G030C8Tx | MCU Cortex-M0+ 64 Ko | `mcu` | LQFP-48 | ST **DS12991 Rev 3** (avril 2020) | 93 |
| J1/J2/J4 | NMJ4HFD2 | Jack 6,35 mm **mono**, à contact commuté | `wired_input` | traversant | Neutrik | 3 |

Le PDF `U2_PCM1863.pdf` est la datasheet de **toute la famille PCM186x** (1860 à 1865) :
bien vérifier la colonne / la note qui s'applique au **PCM1863** avant de conclure.
Idem `U6_TLV767.pdf`, qui couvre toutes les tensions de sortie de la famille TLV767 —
la variante du projet est le **TLV76733** (3,3 V fixe).

## Où chercher quoi (numéros de page **du PDF**)

| Réf. | Brochage | Max absolus | Cond. op. | Carac. élec. | Thermique | Application | Layout | Registres |
|---|---|---|---|---|---|---|---|---|
| U1 | 9 | 28 | 29 | — | — | — | 37 | — |
| U2 | 8 | 12 | 12 | 13 | 12 | 70 | 84 | 85 |
| U3 | 4 | 7 | 7 | 8 | 7 | 60 | 71 | 73 |
| U4 | 3 | 5 | 5 | 6 | 5 | 25 | 27 | — |
| U5 | 3 | 5 | 6 | 7 | 6 | 22 | 33 | — |
| U6 | 3 | 5 | 5 | 6 | 6 | 1 | 24 | — |
| U7 | 3 | 4 | 4 | 5 | 4 | 15 | 26 | — |
| U8 | 2 (config) / 12 (fonctions) | 2 | — | 5 | — | 14 | — | — |
| U9 | **29** (fig. 3, LQFP48) / **31** (tab. 12, affectation) | 40 | 41 | 38 | 41 | — | — | — |

## Documents conservés à titre de comparaison

`_FSC-BT1026x_datasheet.pdf` et `_FSC-BT1026x_userguide.pdf` : ce sont les documents
d'un **autre module** (BT1026x), présents dans le projet avant que la datasheet du
BT1058 ne soit récupérée. Le schéma et l'empreinte disent **FSC-BT1058** ; c'est
`U1_FSC-BT1058.pdf` qui fait foi. Les fichiers `_`-préfixés sont gardés le temps de
confirmer le brochage réellement câblé, puis à supprimer.

## Réserves sur les versions

- **U8 (LT3042)** : révision `3042fa`, récupérée via le miroir Farnell — `analog.com`
  est injoignable depuis ce poste (la connexion TCP vers l'edge Akamai expire). La
  révision courante chez ADI est `3042fb`. Vérifier sur un poste connecté avant de
  figer des valeurs limites.
- **U9 (STM32G030)** : `DS12991 Rev 3` (avril 2020), miroir LCSC — `st.com` est
  injoignable de la même façon. La révision courante est Rev 6 (juin 2025). Le
  brochage LQFP48 ne change pas entre révisions, les caractéristiques électriques
  peuvent bouger à la marge.
- Le **manuel de référence RM0454** (registres STM32G0, ~990 p.) n'est pas ici : il
  sert au firmware, pas à la revue de schéma. À ajouter le jour où on écrit les drivers.
- Le champ *Datasheet* du symbole **U2** dans KiCad pointe vers `pcm1862.pdf` alors que
  la valeur est `PCM1863DBT` — sans conséquence électrique (même document TI), mais à
  corriger pour éviter la confusion.

## Provenance

Récupérés le 2026-08-29 depuis : `ti.com/lit/ds/symlink/…` (U4, U6, U7 + U2),
`document.feasycom.com` (U1), `farnell.com/datasheets/1941078.pdf` (U8),
`wmsc.lcsc.com` (U9), `neutrik.com` (J1/J2/J4). U3 et U5 étaient déjà dans le dépôt.
