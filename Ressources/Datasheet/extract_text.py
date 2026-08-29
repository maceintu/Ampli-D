"""Extrait le texte de chaque datasheet PDF en .txt greppable (une balise par page).

    python extract_text.py            # (re)genere txt/ pour tous les PDF du dossier
    python extract_text.py U5         # limite aux PDF dont le nom contient "U5"

Le .txt est la version "Claude friendly" : cherchable avec grep -n, sans avoir a
relancer un parseur PDF. Les schemas et courbes ne sont evidemment pas repris —
pour ceux-la, ouvrir le PDF.
"""
import os, sys, glob
import pypdf

sys.stdout.reconfigure(encoding='utf-8', errors='replace')
here = os.path.dirname(os.path.abspath(__file__))
out = os.path.join(here, 'txt')
os.makedirs(out, exist_ok=True)
filt = sys.argv[1] if len(sys.argv) > 1 else ''

for pdf in sorted(glob.glob(os.path.join(here, '*.pdf'))):
    name = os.path.splitext(os.path.basename(pdf))[0]
    if filt and filt not in name:
        continue
    dst = os.path.join(out, name + '.txt')
    try:
        reader = pypdf.PdfReader(pdf)
    except Exception as e:
        print(f'{name:<28} ERREUR {e}')
        continue
    with open(dst, 'w', encoding='utf-8') as f:
        f.write(f'# {name} — {len(reader.pages)} pages\n')
        for i, page in enumerate(reader.pages, 1):
            f.write(f'\n\n===== page {i} =====\n')
            f.write(page.extract_text() or '')
    print(f'{name:<28} {len(reader.pages):>4} p  ->  txt/{name}.txt '
          f'({os.path.getsize(dst)//1024} Ko)')
