package classpath;

import java.io.File;
import java.util.Map;

public class ClassPath {
    private Entry bootClassPath;
    private Entry extClassPath;
    private Entry userClassPath;

    public ClassPath(String jreOption,String cpOption) {
        parseBootAndExtClassPath(jreOption);
        parseUserClassPath(cpOption);
    }

    private void parseBootAndExtClassPath(String jreOption) {
            String jreDir = getJreDir(jreOption);
            if(jreDir == null) {
                System.err.println("Can not find jre folder!");
                System.exit(-1);
            }

            // jre/lib/*
            String jreLibPath = jreDir + File.separator + "lib" + File.separator + "*";
            bootClassPath = Entrys.newWildcardEntry(jreLibPath);

            // jre/lib/ext/*
            String jreExtPath = jreDir + File.separator + "lib" + File.separator + "ext" + File.separator + "*";
            extClassPath = Entrys.newWildcardEntry(jreExtPath);
    }

    private void parseUserClassPath(String cpOption) {
        if(cpOption == null || cpOption.equals("")) {
            cpOption = ".";
        }
        userClassPath = Entrys.newEntry(cpOption);
    }

    private String getJreDir(String jreOption) {
        if(jreOption != null && !jreOption.equals("") && exist(jreOption)) {
            return jreOption;
        }
        if(exist("./jre")) {
            return ".jre";
        }
        Map<String,String> env = System.getenv();
        String jh = env.get("JAVA_HOME");
        if(jh != null) {
            return jh + File.separator + "jre";
        }
        return null;
    }

    private boolean exist(String jreOption) {
        File file = new File(jreOption);
        return file.exists();
    }

    public Entry getBootClassPath() {
        return bootClassPath;
    }

    public Entry getExtClassPath() {
        return extClassPath;
    }

    public Entry getUserClassPath() {
        return userClassPath;
    }

    public byte[] readClass(String className){
        className = className + ".class";
        byte[] content;


        content = bootClassPath.readClass(className);
        if(content != null) {
            return content;
        }

        content = extClassPath.readClass(className);
        if(content != null) {
            return content;
        }

        return userClassPath.readClass(className);
    }
}