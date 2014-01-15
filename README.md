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

    > tokenize TokenFriso "Groonga是一款可嵌入式的全文搜寻引擎"
    [
      [
        0,
        0.0,
        0.0
      ],
      [
        {
          "value": "Groonga",
          "position": 0
        },
        {
          "value": "是",
          "position": 1
        },
        {
          "value": "一款",
          "position": 2
        },
        {
          "value": "可",
          "position": 3
        },
        {
          "value": "嵌入",
          "position": 4
        },
        {
          "value": "式",
          "position": 5
        },
        {
          "value": "的",
          "position": 6
        },
        {
          "value": "全文",
          "position": 7
        },
        {
          "value": "搜寻引擎",
          "position": 8
        }
      ]
    ]
  
## License

LGPLv2.1. It is same as Groonga.
