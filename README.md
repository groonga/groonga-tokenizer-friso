# Friso tokenizer for Groonga

## Install

Install libgroonga-dev, friso in the advance.

* Groonga http://groonga.org/
* Friso https://code.google.com/p/friso/

Build this tokenizer.

    % ./configure --with-friso=PATH_TO_LIB_DIR --with-friso-ini=PATH_TO_INI_FILE
    % make
    % sudo make install

## Usage

Register `tokenizers/friso`:

    % groonga DB
    > register tokenizers/friso
    [[0,0.0,0.0],true]

Now, you can use `TokenFriso` that tokenizes the sentence by each word segmentation:

    > tokenize TokenFriso "我到北京天安门"
    [
      [
        0,
        0.0,
        0.0
      ],
      [
        {
          "value": "我",
          "position": 0
        },
        {
          "value": "到",
          "position": 1
        },
        {
          "value": "北京",
          "position": 2
        },
        {
          "value": "天安门",
          "position": 3
        }
      ]
    ]

## License

LGPLv2.1. It is same as Groonga.
