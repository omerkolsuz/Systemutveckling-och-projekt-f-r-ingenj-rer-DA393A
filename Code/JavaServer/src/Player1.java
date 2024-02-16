import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.List;

public class Player1 { //författare: Oscar, Bilal, Firoz
    private static JTextArea highscoreTextArea;

    public static void main(String[] args) {

        Thread serverThread = new Thread(() -> startServer());
        serverThread.start();


        SwingUtilities.invokeLater(() -> createAndShowGUI());
    }

    //Instansvariablar nedan, författare: Oscar, Bilal, Firoz
    private static String clientUsername;
    private static String tries;

    //författare Bilal Ayubi
    private static void startServer() {
        try {
            ServerSocket serverSocket = new ServerSocket(3001);
            System.out.println("Server started, waiting for connection...");

            while (true) {

                Socket arduinoSocket = serverSocket.accept();
                System.out.println(arduinoSocket.getInetAddress() + " connected");


                Thread arduinoThread = new Thread(() -> processArduinoConnection(arduinoSocket));
                arduinoThread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //författare Bilal Ayubi, Oscar Svantesson
    private static void processArduinoConnection(Socket arduinoSocket) {
        try {

            InputStream arduinoInputStream = arduinoSocket.getInputStream();
            OutputStream arduinoOutputStream = arduinoSocket.getOutputStream();

            while (clientUsername == null) {

                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            String response = "Username: " + clientUsername;

            arduinoOutputStream.write(response.getBytes());
            arduinoOutputStream.flush();
            System.out.println("Sent clientUsername to Arduino: " + response);

            while (true) {
                byte[] buffer = new byte[1024];
                int numBytes = arduinoInputStream.read(buffer);
                if (numBytes > 0) {
                    String message = new String(buffer, 0, numBytes);
                    System.out.println("Received message: " + message);
                    if (message.contains("Tries:")) {
                        tries = message;
                        System.out.println(tries);

                        writeTriesToFile();
                    }



                    System.out.println("Client Username: " + clientUsername);


                    SwingUtilities.invokeLater(() -> updateHighscoreList(message));
                } else {

                    break;
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }




    //författare: Oscar Svantesson, Bilal Ayubi, Firoz Akbari
    private static void createAndShowGUI() {

        JFrame frame = new JFrame("Player 1");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);
        frame.getContentPane().setBackground(Color.WHITE);
        frame.setLayout(new BorderLayout());

        JPanel usernamePanel = new JPanel();
        usernamePanel.setBackground(Color.WHITE);
        frame.add(usernamePanel, BorderLayout.NORTH);

        JLabel usernameLabel = new JLabel("Username: ");
        usernamePanel.add(usernameLabel);

        JTextField usernameField = new JTextField(20);
        usernamePanel.add(usernameField);

        JPanel highscorePanel = new JPanel(new BorderLayout());
        highscorePanel.setBackground(Color.WHITE);
        frame.add(highscorePanel, BorderLayout.CENTER);

        JLabel highscoreLabel = new JLabel("Highscores:");
        highscorePanel.add(highscoreLabel, BorderLayout.NORTH);

        highscoreTextArea = new JTextArea(10, 40);
        highscoreTextArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(highscoreTextArea);
        highscorePanel.add(scrollPane, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel();
        buttonPanel.setBackground(Color.WHITE);
        frame.add(buttonPanel, BorderLayout.SOUTH);


        JButton connectButton = new JButton("Connect");
        connectButton.addActionListener(e -> {
            String username = usernameField.getText();
            if (!username.isEmpty()) {

                Thread javaThread = new Thread(() -> startJavaClient(username));
                javaThread.start();


                usernameField.setEnabled(true);
                connectButton.setEnabled(true);
            }
        });
        buttonPanel.add(connectButton);


        JButton refreshButton = new JButton("Refresh");
        refreshButton.addActionListener(e -> refreshHighscores());
        buttonPanel.add(refreshButton);

        frame.setVisible(true);
    }
    //Författare Oscar, Firoz
    private static void refreshHighscores() {
        highscoreTextArea.setText("");
        try {
            FileReader fileReader = new FileReader("highscores.txt");
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String message;
            List<String> highscores = new ArrayList<>();
            while ((message = bufferedReader.readLine()) != null) {
                highscores.add(message);
            }
            bufferedReader.close();

            highscores.sort(Comparator.comparingInt(Player1::extractTries));

            for (String highscore : highscores) {
                updateHighscoreList(highscore);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    private static void startJavaClient(String username) { //författare Bilal Ayubi
        try {
            clientUsername = username;

            Socket serverSocket = new Socket("localhost", 3001);
            System.out.println("Connected to server: " + serverSocket.getInetAddress());


            OutputStream outputStream = serverSocket.getOutputStream();


            outputStream.write(username.getBytes());
            outputStream.flush();


            FileReader fileReader = new FileReader("highscores.txt");
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String message;
            List<String> highscores = new ArrayList<>();
            while ((message = bufferedReader.readLine()) != null) {
                highscores.add(message);
            }

            highscores.sort(Comparator.comparingInt(Player1::extractTries));


            for (String highscore : highscores) {
                SwingUtilities.invokeLater(() -> updateHighscoreList(highscore));
            }


            bufferedReader.close();

            serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //Författare Oscar, Firoz
    private static void updateHighscoreList(String message) {

        highscoreTextArea.append(message + "\n");
        highscoreTextArea.setCaretPosition(highscoreTextArea.getDocument().getLength());
    }
    //Författare Oscar, Firoz
    private static void writeTriesToFile() {
        try {
            String newEntry = tries.trim();
            File file = new File("highscores.txt");
            if (file.exists()) {

                FileReader fileReader = new FileReader(file);
                BufferedReader bufferedReader = new BufferedReader(fileReader);
                String line;
                while ((line = bufferedReader.readLine()) != null) {
                    String entry = line.trim();
                    if (entry.equals(newEntry)) {

                        bufferedReader.close();
                        return;
                    }
                }
                bufferedReader.close();
            }

            FileWriter fileWriter = new FileWriter(file, true);
            BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
            bufferedWriter.write(newEntry);
            bufferedWriter.newLine();
            bufferedWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //Författare Oscar, Firoz
    private static int extractTries(String highscore) {

        String[] parts = highscore.split("Tries:");
        if (parts.length == 2) {
            try {
                return Integer.parseInt(parts[1].trim());
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }
        return 0;
    }
}

