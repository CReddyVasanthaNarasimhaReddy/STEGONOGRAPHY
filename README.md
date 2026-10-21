
# Image Steganography Using LSB in C

## 👨‍💻 Author Details

- **Name:** C Reddy Vasantha Narasimha Reddy
- **Email:** [crvnreddy48@gmail.com](mailto:crvnreddy48@gmail.com)
- **LinkedIn:** [Connect with me](https://www.linkedin.com/in/reddy-vasantha-narasimha-reddy-c-690279352/)
- **Programming Language:** C

---

## 📌 Project Description

Image Steganography is a C-based application that hides secret information inside a BMP image and extracts it when required. The project uses the **Least Significant Bit (LSB) technique** to embed secret data into image pixel bytes.

Unlike encryption, which transforms information into an unreadable format, steganography hides the existence of the secret information inside another file.

The application supports encoding a secret file into a BMP image and decoding the hidden data to recover the original file.

## ✨ Features

### 🔐 Encoding
- Accepts a source BMP image and a secret file.
- Validates input filenames and file extensions.
- Checks whether the image has sufficient capacity.
- Copies the BMP header to the output image.
- Encodes a magic string to identify the hidden data.
- Embeds the secret file extension and file size.
- Stores the secret file contents in the image using LSB manipulation.
- Generates a stego image containing the hidden information.

### 🔓 Decoding
- Reads the stego BMP image.
- Identifies the hidden information using a magic string.
- Extracts the secret file extension and size.
- Recovers the embedded file contents.
- Saves the recovered information into an output file.

## 🛠️ Technologies Used

- **Programming Language:** C
- **Compiler:** GCC
- **Development Environment:** Visual Studio Code / Linux Terminal
- **Image Format:** BMP
- **Technique:** Least Significant Bit (LSB) Steganography

## 📚 Concepts Used

- File handling using `fopen()`, `fread()`, `fwrite()`, and `fclose()`
- Structures and enumerations
- Functions and function pointers
- Pointers and arrays
- String handling
- Bitwise operators and bit manipulation
- Command-line arguments
- File size calculation and validation
- Modular programming

## 📂 Project Structure

```text
Image-Steganography/
├── main.c
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── types.h
├── common.h
├── beautiful.bmp
├── secret.txt
├── stego.bmp
├── Output.txt
└── README.md
```

## ⚙️ Compilation

Compile the source files using GCC:

```bash
gcc main.c encode.c decode.c -o steganography
```

Make sure all required source and header files are present in the project directory.

## ▶️ How to Run

### 1. Encode a Secret File

Use the following command to hide `secret.txt` inside `beautiful.bmp`:

```bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```

**Explanation:**
- `-e`: Selects encoding mode.
- `beautiful.bmp`: Source image.
- `secret.txt`: File containing secret information.
- `stego.bmp`: Output image containing the hidden data.

### 2. Decode the Secret File

Use the following command to extract the hidden information:

```bash
./steganography -d stego.bmp
```

The program extracts the hidden file and saves it using the default output filename based on the embedded file extension.

To specify an output filename, use:

```bash
./steganography -d stego.bmp recovered.txt
```

The output filename extension should match the embedded secret file extension.

## 💻 Sample Execution

### Encoding

**Command:**

```bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```

**Illustrative Output:**

```text
INFO: Arguments validated successfully
INFO: Opening source image file
INFO: Checking capacity of source image
INFO: Encoding magic string
INFO: Encoding secret file extension
INFO: Encoding secret file size
INFO: Encoding secret file data
INFO: Encoding completed successfully
```

### Decoding

**Command:**

```bash
./steganography -d stego.bmp
```

**Expected Result:**

```text
INFO: Opening stego image file
INFO: Decoding magic string
INFO: Decoding secret file extension
INFO: Decoding secret file size
INFO: Decoding secret file data
INFO: Decoding completed successfully
```

*Note: Sample output is illustrative; actual messages depend on the implementation.*

## 🎯 Project Objectives

- To understand the fundamentals of image steganography.
- To implement data hiding using the LSB technique.
- To encode and decode secret files within BMP images.
- To develop practical knowledge of binary file handling in C.
- To understand bit manipulation and command-line argument processing.
- To recover hidden information from a stego image.

## 🚀 Future Enhancements

- Support for additional image formats.
- Improved error handling and input validation.
- Support for larger secret files.
- Password-based protection for hidden data.
- Integration with encryption for improved confidentiality.
- A graphical user interface for easier operation.

## 📖 Learning Outcomes

This project provides practical experience in C programming, binary file operations, bitwise manipulation, structures, and modular software development. It demonstrates how secret data can be embedded into and extracted from image files using the LSB steganography technique.

## 🤝 Connect With Me

- **Email:** [crvnreddy48@gmail.com](mailto:crvnreddy48@gmail.com)
- **LinkedIn:** [C Reddy Vasantha Narasimha Reddy](https://www.linkedin.com/in/reddy-vasantha-narasimha-reddy-c-690279352/)

---

⭐ Thank you for visiting my project!

**Developed by C Reddy Vasantha Narasimha Reddy**
