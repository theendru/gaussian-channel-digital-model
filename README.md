# Цифровая модель гауссовского канала связи для QAM-сигнала
Представленный проект это реализация функций QAM модулятора, QAM демодулятора и добавления гауссовского шума.
<img width="752" alt="image" src="https://github.com/user-attachments/assets/1eebbe70-d327-4440-b56a-8ce12771daad">
## `QAMmodulator.h` `QAMdemodulator.h` `GaussianChannel.h`
Имплементация каждой из вышеперечисленных функций представляет из себя соответсвующие классы и расположены в заголовочных файлах `QAMmodulator.h`, `QAMdemodulator.h` и `GaussianChannel.h`.
## `Instruments.h`
Здесь содержатся функции, необходимые для удобного проведения экспериментов. В основном, это касается работы с программой из консоли.
## `GaussianChannelDigitalModel.cpp`
Это основная часть проекта. Здесь проводятся эксперименты по вычислению битовой ошибки (BER) для разных порядков модуляций: 4-QAM (aka QPSK), 16-QAM, 64-QAM. Стоит заметить, что все эти созвездия квадратные. Полученные значения BER вместе со значениями порядков модуляции и значениями отношения сигнал шум (SNR) помещаются в промежуточный файл `BERdata.csv`.
## `GaussianChannelDigitalModelApp.mlapp`
MATLAB-часть проекта необходима для построения семейства кривых помехоустойчивости. При нажатии кнопки `Start` в окне программы происходит компиляция `GaussianChannelDigitalModel.cpp` и его запуск. Затем читается файл `BERdata.csv` и выводятся графики кривых помехоустойчивости для каждого из порядков модуляции. Повторное нажатие на `Start` повторяет всю процедуру.\
**Note:** между нажатием кнопки `Start` и выводом графиков проходит некоторое время, которое не сопровождается выводом какой-либо информации.\
**Note:** в процессе может возникнуть ошибка компиляции основного *.cpp*-файла. Именно по этой причине был загружен файл с результатами, чтобы не смотря ни на что кривые были построены. Для того, чтобы обойти компиляцию из MATLAB, необходимо самостоятельно запустить `GaussianChannelDigitalModel.cpp`, затем проверить, что файл `BERdata.csv` перезаписался и запустить `GaussianChannelDigitalModelApp.mlapp`.
