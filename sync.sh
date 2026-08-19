#!/bin/bash
set -e

echo "🔍 1. Controllo e salvataggio modifiche locali..."
if [[ -n $(git status -s) ]]; then
    echo "📋 Differenze rilevate:"
    git diff --stat
    echo "💾 Salvataggio automatico in un commit locale..."
    git add .
    git commit -m "Auto-save personalizzazioni varianti ($(date +'%Y-%m-%d %H:%M'))"
else
    echo "✅ Nessuna modifica locale pendente."
fi

echo "📡 2. Controllo aggiornamenti da remoto..."
git fetch origin main

# Confronta il commit locale con quello remoto
LOCAL=$(git rev-parse HEAD)
REMOTE=$(git rev-parse origin/main)

if [ "$LOCAL" != "$REMOTE" ]; then
    echo "🚀 Nuovi commit trovati. Sincronizzazione in corso..."
    # --rebase applica i tuoi commit locali IN CIMA agli aggiornamenti scaricati
    git pull --rebase origin main
else
    echo "✨ Il codice base è già allineato a origin/main."
fi

echo "🛠️ 3. Test di compilazione PlatformIO..."
pio run -e heltec_v3

echo "🎉 Aggiornamento e verifica completati con successo!"