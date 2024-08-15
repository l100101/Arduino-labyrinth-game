def transliterate(text):
    replacements = {
        'а': 'a', 'б': '6', 'в': 'B', 'г': 'r', 'д': 'D', 'е': 'e', 'ё': 'e', 'ж': '}I{',
        'з': '3', 'и': 'u', 'й': 'u', 'к': 'k', 'л': 'JI', 'м': 'M', 'н': 'H', 'о': 'o',
        'п': 'n', 'р': 'p', 'с': 'c', 'т': 'T', 'у': 'y', 'ф': 'O', 'х': 'x', 'ц': 'U',
        'ч': '4', 'ш': 'w', 'щ': 'w', 'ъ': '`', 'ы': 'bI', 'ь': 'b', 'э': '3', 'ю': 'IO',
        'я': '9', 'А': 'A', 'Б': 'B', 'В': 'B', 'Г': 'r', 'Д': 'D', 'Е': 'E', 'Ё': 'E',
        'Ж': '}I{', 'З': '3', 'И': 'u', 'Й': 'u', 'К': 'K', 'Л': 'JI', 'М': 'M', 'Н': 'H',
        'О': 'O', 'П': 'n', 'Р': 'p', 'С': 'c', 'Т': 'T', 'У': 'y', 'Ф': 'O', 'Х': 'X',
        'Ц': 'U', 'Ч': '4', 'Ш': 'w', 'Щ': 'w', 'Ъ': '`', 'Ы': 'bI', 'Ь': 'b', 'Э': '3',
        'Ю': 'I0', 'Я': '9I', ' ': ' ', ',': ',', '.': '.', '?': '?', '!': '!', ':': ':',
        ';': ';', '-': '-'
    }
    transliterated_text = ''
    for char in text:
        if char in replacements:
            transliterated_text += replacements[char]
        else:
            transliterated_text += char
    return transliterated_text

def generate_progmem_string(input_text, variable_name):
    transliterated_text = transliterate(input_text)
    progmem_string = 'const char {}[] PROGMEM = "{}";'.format(variable_name, transliterated_text)
    return progmem_string

def main():
    input_text = input("Введите текст для конвертации в PROGMEM: ")
    variable_name = input("Введите имя переменной: ")

    progmem_string = generate_progmem_string(input_text, variable_name)
    print(progmem_string)

    output_file_name = "conv.txt"
    with open(output_file_name, 'w') as output_file:
        output_file.write(progmem_string)
    print(f"Результат сохранен в файл {output_file_name}")
    
if __name__ == "__main__":
    main()
