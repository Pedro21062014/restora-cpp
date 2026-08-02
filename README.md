# Restora C++ - File Recovery Tool

**Versão nativa em C++ puro** - Sem frameworks, sem bloat, funciona em PCs antigos com apenas 2GB RAM.

## 🚀 Download

### Windows (Executável Direto)
- **[Restora-x86.exe](https://github.com/Pedro21062014/restora/releases/download/v1.1.4/Restora-x86.exe)** - 32-bit (PCs antigos)
- **[Restora.exe](https://github.com/Pedro21062014/restora/releases/download/v1.1.4/Restora.exe)** - 64-bit

### Instaladores
- **Restora_1.1.3_x64-setup.exe** - Windows 64-bit
- **Restora_1.1.3_x86-setup.exe** - Windows 32-bit

### Linux
- **restora_1.1.3_amd64.deb** - Debian/Ubuntu
- **restora_1.1.3_amd64.AppImage** - Qualquer distro
- **restora-1.1.3-1.x86_64.rpm** - RPM

### macOS
- **Restora_1.1.3_aarch64.dmg** - Apple Silicon

---

## ✨ Features

- 🔍 **Scan Rápido e Profundo** - Encontra arquivos deletados rapidamente
- 🖼️ **Recuperação de Imagens** - JPG, PNG, GIF, BMP, WEBP, TIFF
-  **Recuperação de Vídeos** - MP4, AVI, MKV, MOV
- 🎵 **Recuperação de Áudio** - MP3, WAV, FLAC, OGG
- 📄 **Recuperação de Documentos** - PDF, DOC, DOCX, XLSX, PPTX
- 📦 **Recuperação de Arquivos** - ZIP, RAR, 7Z
- 🔧 **Reparo Automático** - Conserta arquivos danificados
- ️ **Filtro de Thumbnails** - Evita salvar previews
- 💾 **Leve** - Funciona com 2GB RAM
- ️ **32-bit** - Suporte para PCs antigos

---

## 🔧 Compilação

### Requisitos
- CMake 3.10+
- C++17 compiler (MSVC, GCC, Clang)
- Windows SDK (para Windows)

### Windows (MSVC)
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Windows (MinGW)
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

### Linux
```bash
mkdir build
cd build
cmake ..
make
```

---

##  Uso

### Interface Console
```
1. Scan Drive     - Escanear disco por arquivos recuperáveis
2. List Drives    - Listar todos os discos disponíveis
3. Settings       - Configurações do scan
4. About          - Informações sobre o programa
0. Exit           - Sair
```

### Exemplo de Uso
```
Restora.exe

══════════════════════════════════════════════════════╗
║         RESTORA - File Recovery Tool v1.1.4          ║
║    Lightweight • Fast • Minimal • Works on 2GB RAM   ║
══════════════════════════════════════════════════════╝

══════════════════════════════════════════════════════╗
║                    MAIN MENU                         ║
╠══════════════════════════════════════════════════════
║  1. Scan Drive                                       ║
║  2. List Drives                                      ║
║  3. Settings                                         ║
║  4. About                                            ║
║  0. Exit                                             ║
╚══════════════════════════════════════════════════════╝

Enter choice: 1
```

---

## ⚠️ Windows 7/8/8.1

Se aparecer erro `api-ms-win-crt-math-l1-1-0.dll is missing`:

1. Baixe: https://aka.ms/vs/17/release/vc_redist.x64.exe
2. Instale e reinicie
3. Execute o Restora novamente

---

## 🏗️ Build CI/CD

O projeto usa GitHub Actions para compilação automática:
- Windows 32-bit e 64-bit
- Linux (deb, AppImage, rpm)
- macOS (DMG)

---

## 📄 License

MIT © 2026 Restora

---

##  Créditos

Desenvolvido em C++ puro com Windows API.
Sem dependências de frameworks web.
Compatível com PCs de 15+ anos atrás.
