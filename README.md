### Juego Traffic Racer

### 🎯 Objetivo del Juego

Traffic Racer es un juego de conducción en una carretera infinita donde controlas un coche esquivando el tráfico. El objetivo es sobrevivir el mayor tiempo posible para acumular la máxima puntuación y establecer nuevos récords. 

Debes gestionar estratégicamente tu combustible: evitar chocar con otros vehículos, mantenerte dentro de la carretera y recolectar bidones de gasolina en el camino. Cada segundo que sobrevives suma puntos, pero cualquier error reduce tu combustible rápidamente. ¿Cuánto tiempo podrás resistir?

### 🎮 Controles

- **← / →** (Flechas izquierda/derecha): Movimiento del coche
- **↑ / ↓** (Flechas arriba/abajo): Navegar en los menús
- **Enter**: Seleccionar opciones en menús
- **ESC / P**: Pausar el juego
- **F11**: Alternar entre pantalla completa y modo ventana

**Funcionalidades ocultas** (no se muestran al usuario en el juego):
- **F1**: Mostrar/ocultar hitboxes (modo debug)
- **Delete**: Resetear el high score almacenado en archivo .dat (solo en menú principal)

### ⚙️ Mecánicas

**Sistema de Combustible:**
- Tanque inicial de 100 unidades que se consume constantemente (5 unidades/segundo)
- Colisiones con otros vehículos reducen 32 unidades
- Salirse de la carretera penaliza con 12 unidades iniciales + 18 unidades/segundo continuo
- Items de gasolina recargan 35 unidades

**Sistema de Puntuación:**
- 25 puntos por cada segundo sobrevivido
- High score persistente guardado automáticamente
- Detección de nuevo récord con efectos especiales

**Dificultad Progresiva:**
- Sistema gradual tipo "T-Rex de Google Chrome"
- Cada 60 puntos aumenta el nivel de dificultad
- Velocidad de enemigos incrementa hasta 2.5x
- Frecuencia de aparición aumenta progresivamente
- Consumo de combustible se acelera en niveles avanzados (nivel 25+)

**Tráfico y Obstáculos:**
- 4 carriles de tráfico (2 en cada dirección)
- 7 tipos diferentes de vehículos enemigos
- Camiones especiales que ocupan 2 carriles completos
- Spawn inteligente que evita colisiones injustas

### 🏆 Características

**Elementos que hacen único a este Traffic Racer:**

- **6 Temas Visuales Dinámicos:** El escenario cambia cada 500 puntos alternando entre Normal, Desierto, Jungla, Montañas, Nevado y Playa, con transiciones suaves tipo fade

- **Sistema de Combustible Estratégico:** A diferencia del juego original, tu vida depende de la gasolina, creando un balance entre riesgo y supervivencia

- **Dificultad Adaptativa:** El juego se vuelve progresivamente más difícil de forma casi imperceptible, similar al juego del dinosaurio de Chrome

- **Audio Inmersivo:** Sonidos de motor realistas, música de fondo dinámica que se ajusta según el estado del juego (menú/juego/pausa)

- **Camiones Especiales:** Vehículos de gran tamaño que ocupan 2 carriles y requieren mayor habilidad para esquivar

- **Interfaz Escalable:** Soporte para pantalla completa y modo ventana con UI que se adapta perfectamente a cualquier resolución

- **Sistema de Récords:** Persistencia de puntuación máxima con celebración especial al romper el récord

- **Cuenta Regresiva Inicial:** Preparación de 3 segundos antes de comenzar cada partida para mayor inmersión

### 👥 Equipo

- **Líder**: Raúl Alejandro Jiménez Gómez (@RaulAlejandroJimenezGomez)
- **Integrante 2**: Jorge Alberto Celis Pérez (@JorgeCelisPerez)

### 🛠️ Tecnologías

- Motor/Framework: SFML (Simple and Fast Multimedia Library)
- Lenguaje: C++ (estándar C++17)
- Librerías adicionales:
  - SFML Graphics (renderizado y gráficos)
  - SFML Window (gestión de ventana)
  - SFML System (utilidades del sistema)
  - SFML Audio (sonidos y música)
  - Librerías estándar de C++: iostream, cstdlib, algorithm, sstream, fstream, cmath

### 📜 Créditos

La mayoría de estos assets (sprites, fuentes y sonidos) salieron de la página opengameart.org y algunos eran de libre uso, pero otros sí pedían atribución, por lo que pondremos todo para darles créditos aunque lo pidieran o no.
        
- Las imágenes de los coches utilizados son del autor sujit1717 en https://opengameart.org/content/free-top-down-car-sprites-by-unlucky-studio

- Para la carretera nos basamos en la imagen del autor Alucard en https://opengameart.org/content/2d-top-down-highway-background, la cual modificamos con Gemini para escalarla y agregarle paisajes, así como para el fondo borroso del menú.

- Las imágenes del teclado son del autor Brett Post Script en https://opengameart.org/content/pc-keyboard-mouse-and-touch-input-icons

- El sonido utilizado para el item de la gasolina es del autor Musheran en https://opengameart.org/content/item-pickup-key

- Para el sonido de los choques usamos el de Blender Foundation (Submitted by Lamoot) en https://opengameart.org/content/stop

- Para la cuenta regresiva para iniciar es de kheetor en https://opengameart.org/content/race-start-countdown

- Para la barra de la gasolina utilizamos los archivos de ChaosRobH en https://opengameart.org/content/minimalist-bar-system-0

- Para el sonido de GameOver utilizamos el audio de 0new4y en https://opengameart.org/content/game-over-trumpet-sfx
    
- La fuente utilizada se llama Speed Rusher del autor Arterfak Project sacada de la página https://www.1001fonts.com/users/ramz/

- El sonido de nuevo récord fue sacado de Vocals - Winter X-Games SnoCross - Sound Effects (Arcade) de la página https://www.101soundboards.com/boards/38919-vocals-winter-x-games-snocross-sound-effects-arcade

- Para el sonido de aceleración es del autor freesound_community y sacado de https://pixabay.com/es/sound-effects/car-acceleration-inside-car-7087/

- Para el sonido de rugido que suena al presionar empezar es del autor DRAGON-STUDIO en https://pixabay.com/es/sound-effects/car-engine-roaring-376881/

- Por ultimo la musica de fondo del juego es del autor Cyberwave-Orchestra en https://pixabay.com/es/music/rock-simple-rock-riff-loop-289668/

- Lo demas que no aparece aqui citado fue creado directamente con Gemini.

Agradecimientos

- Se agradece al profesor Jose Ramon Navarro por el apyo, segimiento y consejos a lo largo del semestre, las cuales sirvieron para dar bases y camino a la creacion de este proyecto y su correcta finalizacion.

- Tambien agradecimientos a Carlo Francisco Vicencio Martinez por la ayuda a lo largo de la realizacion en temas de instalaciones y dudas que iban surguiendo.
