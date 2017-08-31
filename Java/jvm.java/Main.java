import classpath.ClassPath;
import org.apache.commons.cli.*;

public class Main {
    public static void main(String[] args) {
        Options options = new Options();

        Option helpFlag = new Option("h","help",false,"print help message");
        options.addOption(helpFlag);
        Option versionFlag = new Option("version",false,"print version and exit");
        options.addOption(versionFlag);
        Option cp = new Option("cp",true,"classpath");
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

        ClassPath cp = new ClassPath(cmd.getOptionValue("jre"),cmd.getOptionValue("cp"));
        String[] args = cmd.getArgs();
        if(args.length == 0) {
            System.out.println("Usage: java [-options] class [args...]");
            return;
        }

        String className = args[0].replace('.','/');
        byte[] content = cp.readClass(className);
        if(content != null) {
            for(byte b : content) {
                System.out.printf("%d ",b);
            }
            System.out.println();
        }
    }
}    