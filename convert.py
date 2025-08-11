import os
import argparse
import tempfile
import subprocess
from pathlib import Path

def find_folder_structure(top_dir: Path) -> list[Path]:
    """
    Находит все папки, соответствующие заданной структуре
    """
    top_dir = top_dir / "clips"
    matching_folders = [
        os.path.join(top_dir, d) 
        for d in os.listdir(top_dir) 
        if os.path.isdir(os.path.join(top_dir, d))
    ]

    return sorted(matching_folders)

def select_folder(folders: list[Path]) -> Path:
    """
    Позволяет пользователю выбрать папку из списка
    """
    if not folders:
        print("Не найдено подходящих папок.")
        return None
    
    print("Найдены следующие папки:")
    for i, folder in enumerate(folders, 1):
        print(f"{i}. {folder}")
    
    while True:
        try:
            choice = int(input("Выберите номер папки (0 для выхода): "))
            if choice == 0:
                return None
            if 1 <= choice <= len(folders):
                return Path(folders[choice-1])
            print("Некорректный ввод. Попробуйте снова.")
        except ValueError:
            print("Пожалуйста, введите число.")

def sort_files(files):
    """
    Сортирует файлы по имени (можно изменить под свои нужды)
    """
    tmp = sorted(files, key=lambda x: x.name)
    init = tmp.pop()
    tmp.insert(0, init)

    return tmp

def create_file_list(folder: Path, dir: Path) -> list[Path]:
    """
    Создает временный файл со списком файлов для ffmpeg
    """

    out0 = dir / "stream0.m4s"
    out1 = dir / "stream1.m4s"

    stream0 = [f for f in folder.glob("video/*/*stream0*.m4s")]
    sorted_files = sort_files(stream0)
    subprocess.run(['cat', *sorted_files], stdout=open(out0, 'w'))

    stream1 = [f for f in folder.glob("video/*/*stream1*.m4s")]
    sorted_files = sort_files(stream1)
    subprocess.run(['cat', *sorted_files], stdout=open(out1, 'w'))

    return [out0, out1]

def run_ffmpeg(video_file: Path, audio_file: Path, output_file):
    """
    Запускает ffmpeg для склейки файлов
    """
    cmd = [
        'ffmpeg',
        '-i', video_file.as_posix(),
        '-i', audio_file.as_posix(),
        '-c', 'copy',
        '-map', '0:v:0',
        '-map', '1:a:0',
        output_file
    ]
    
    try:
        subprocess.run(cmd, check=True)
        print(f"Файл успешно создан: {output_file}")
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при выполнении ffmpeg: {e}")
    except FileNotFoundError:
        print("ffmpeg не найден. Убедитесь, что он установлен и добавлен в PATH.")

def main():
    consent = input("Данная утилита требует столько же дополнительного свободного места на диске, сколько занимает видеофайл. Продолжить? (y/n)")
    if consent.lower() != 'y':
        return

    parser = argparse.ArgumentParser(description='Скрипт для обработки видеофайлов через ffmpeg')
    parser.add_argument('root_dir', help='Корневая директория для поиска')
    
    args = parser.parse_args()
    
    # Шаг 1: Поиск папок с заданной структурой
    matching_folders = find_folder_structure(Path(args.root_dir))
    
    # Шаг 2: Выбор папки пользователем
    selected_folder = select_folder(matching_folders)
    if not selected_folder:
        return
    
    # Шаг 3: Запрос имени выходного файла
    output_filename = input("Введите имя выходного файла (например output.mp4): ")
    if not output_filename:
        print("Имя файла не может быть пустым.")
        return
    
    # Шаг 4-5: Сканирование и сортировка файлов, создание временного файла
    temp_dir = tempfile.gettempdir()

    stream0, stream1 = create_file_list(selected_folder, Path("."))
    
    # Шаг 6: Запуск ffmpeg
    output_path = os.path.join(os.getcwd(), output_filename)
    run_ffmpeg(stream0, stream1, output_path)
    
    # Шаг 7: Очистка временных файлов
    try:
        os.remove(stream0)
        os.remove(stream1)
    except OSError as e:
        print(f"Ошибка при удалении временного файла: {e}")

if __name__ == "__main__":
    main()


# ffmpeg -f concat -safe 0 -i <(for f in clips/clip_2379780_20250807_103928/video/fg_2379780_20250807_102848/*.m4s; do echo "file '$PWD/$f'"; done) -c copy output.mp4