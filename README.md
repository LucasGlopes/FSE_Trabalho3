# Trabalho 3 - Fundamentos de Sistemas Embarcados

## Alunos
|Matrícula | Aluno |
| -- | -- |
| 170010341  |  Gabriel Davi Silva Pereira |
| 170080102  |  Lucas Gomes Lopes |

O objetivo deste trabalho é criar sensores e atuadores distribuídos baseados nos microcontroladores ESP32 interconectados via Wifi através do protocolo MQTT.
Os microcontroladores ESP32 irão controlar a aquisição de dados de sensores, botões e chaves e acionar saídas como leds, dentre outras.Toda a comunicação será feita via rede Wifi com o protocolo MQTT e será provido um servidor central para cadastro e controle dos dispositivos através da plataforma Thingsboard.
</br>O enunciado completo do trabalho pode ser lido [aqui](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-3-2022-1). 


## Componentes do sistema

- **Dev Kit ESP32**
- **Sensor de temperatura e umidade (DHT-11)**
- **Buzzer**

## Informações

O projeto foi desenvolvido usando a ESP-IDF nativa.</br>
Versão utilizada: ESP-IDF v5.1-dev-644-g867745a05c.

## Utilização

Clone o projeto com o comando abaixo:
```
git@github.com:LucasGlopes/FSE_Trabalho3.git
```

Acesse o diretório do projeto:
```
cd FSE_Trabalho3
```
Acesse o arquivo **/.vscode/c_cpp_properties.json** para alterar as variáveis IDF_PATH e IDF_TOOLS para a sua máquina.

Realize o build do projeto:
```
idf.py build
```

Para inserir as credenciais de acesso ao Wifi, digite o comando:
```
idf.py menuconfig
```
Após a aparição do menu, selecione a opção **Configuração da Aplicação**:
![Screenshot from 2022-09-22 21-31-53](https://user-images.githubusercontent.com/49499946/191873674-4dd92cf4-5a5d-4e5f-8ccb-4030357074bb.png)

Insira suas credenciais (WiFi SSID e senha do Wifi) e salve as alterações:
![Screenshot from 2022-09-22 21-32-04](https://user-images.githubusercontent.com/49499946/191873960-834ecb84-56e9-4dbe-bcd3-c9c9f36ecd63.png)

Por fim, rode o programa pelo seguinte comando:
```
idf.py -p (PORTA) flash monitor
```

## Dashboard
![screenshot](https://user-images.githubusercontent.com/49499946/191875178-b84a555c-02e0-4fa7-ba70-9efc86fdf328.png)

## Vídeo
Clique [aqui](https://youtu.be/5pny-U0F4bc) para ver o projeto em execução.
