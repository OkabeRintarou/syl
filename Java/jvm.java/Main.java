import org.apache.commons.cli.*;

public class Main {
    public static void main(String[] args) {
        Options options = new Options();

        Option helpFlag = new Option("h","help",false,"print help message");
        options.addOption(helpFlag);
        Option versionFlag = new Option("version",false,"print version and exit");
        options.addOption(versionFlag);
        Option cp = new Option("classpath",true,"classpath");
        options.addOption(cp);
        cp = new Option("cp",true,"classpath");
        options.addOption(cp);
        Option XjreOption = new Option("Xjre",true,"path ro jre");
        options.addOption(XjreOption);


        CommandLineParser parser = new DefaultParser();
        HelpFormatter formatter = new HelpFormatter();
        CommandLine cmd;

        try {
            cmd = parser.parse(options,args);
        } catch(ParseException e) {
            System.out.println(e.getMessage());
            System.exit(1);
            return;
        }

        if(cmd.hasOption("version")) {
            System.out.println("version 0.0.1");
        } else if(cmd.hasOption("help")) {
            System.out.println("Usage: java [-options] class [args...]");
        } else {
            startJVM(cmd);
        }
    }

    private static void startJVM(CommandLine cmd) {
        System.out.println("Start JVM");
    }
}    