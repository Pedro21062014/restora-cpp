# Restora C++ - File Recovery Tool

**Versão nativa em C++ puro** - Derivado do [Restora original](https://github.com/Pedro21062014/restora), especialmente desenvolvido para **PCs antigos** com Windows 7/8/8.1.

> 💡 **Por que C++ nativo?** O projeto original (Tauri/Electron) requer ~200MB de RAM e o Universal C Runtime. Esta versão C++ nativa requer apenas ~5MB de RAM e funciona em qualquer Windows sem dependências!

##  Download

### Windows (Executável Direto - Sem Instalação!)
- **[Restora-x86.exe](https://github.com/Pedro21062014/restora-cpp/releases/download/v1.0.1/Restora-x86.exe)** - 32-bit (PCs antigos Windows 7/8)
- **[Restora.exe](https://github.com/Pedro21062014/restora-cpp/releases/download/v1.0.1/Restora.exe)** - 64-bit

### Linux
- **restora** - Qualquer distro (compilar com CMake)

### macOS
- **restora** - Apple Silicon (compilar com CMake)

---

## ✨ Features

- 🔍 **Scan Rápido e Profundo** - Encontra arquivos deletados rapidamente
- 🖼️ **Recuperação de Imagens** - JPG, PNG, GIF, BMP, WEBP, TIFF
- 🎬 **Recuperação de Vídeos** - MP4, AVI, MKV, MOV
- 🎵 **Recuperação de Áudio** - MP3, WAV, FLAC, OGG
- 📄 **Recuperação de Documentos** - PDF, DOC, DOCX, XLSX, PPTX
- 📦 **Recuperação de Arquivos** - ZIP, RAR, 7Z
- 🔧 **Reparo Automático** - Conserta arquivos danificados
- ️ **Filtro de Thumbnails** - Evita salvar previews
- 💾 **Ultra Leve** - Apenas ~5MB de RAM (vs ~200MB da versão Tauri)
- 🖥️ **32-bit** - Suporte total para PCs antigos
- ⚡ **Sem Dependências** - Funciona em Windows 7/8/8.1/10/11
- 🔒 **100% Offline** - Não requer internet

## 🖥️ Compatibilidade

| Sistema | Versão | Status |
|---------|--------|--------|
| Windows 11 | 32/64-bit | ✅ Funciona |
| Windows 10 | 32/64-bit | ✅ Funciona |
| Windows 8.1 | 32/64-bit | ✅ Funciona |
| Windows 8 | 32/64-bit | ✅ Funciona |
| Windows 7 | 32/64-bit | ✅ Funciona |

**Requisitos mínimos:**
- Windows 7 ou superior
- 5MB de RAM livre
- 10MB de espaço em disco
- Processador Pentium 4 ou superior

---

## 🔧 Compilação

### Requisitos
- CMake 3.10+
- C++17 compiler (MSVC, GCC, Clang)

### Windows (MinGW) - Recomendado para PCs antigos
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

### Windows (MSVC)
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
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
║         RESTORA - File Recovery Tool v1.0.1          ║
║    Native C++ • Ultra Light • Works on Old PCs       ║
══════════════════════════════════════════════════════╝

══════════════════════════════════════════════════════╗
║                    MAIN MENU                         ║
╠══════════════════════════════════════════════════════
║  1. Scan Drive                                       ║
║  2. List Drives                                      ║
║  3. Settings                                         ║
║  4. About                                            ║
║  0. Exit                                             
╚══════════════════════════════════════════════════════╝

Enter choice: 1
```

---

## 🏗️ Build CI/CD

O projeto usa GitHub Actions para compilação automática:
- Windows 32-bit e 64-bit (MinGW)
- Linux
- macOS

---

##  License

MIT © 2026 Restora

---

##  Créditos

**Derivado de:** [Restora](https://github.com/Pedro21062014/restora) - Versão original em Tauri/React

**Desenvolvido em:** C++ puro com Windows API

**Especialmente para:** PCs antigos com Windows 7/8/8.1

**Compilado com:** MinGW-w64 para máxima compatibilidade
