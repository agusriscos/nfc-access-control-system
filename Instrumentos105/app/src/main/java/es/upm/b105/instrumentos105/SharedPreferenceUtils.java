package es.upm.b105.instrumentos105;

import android.content.Context;
import android.content.SharedPreferences;

public class SharedPreferenceUtils {
    private static SharedPreferenceUtils mSharedPreferenceUtils;
    protected Context mContext;
    private SharedPreferences mSharedPreferences;
    private SharedPreferences.Editor mSharedPreferencesEditor;

    private SharedPreferenceUtils(Context context) {
        mContext = context;
        mSharedPreferences = context.getSharedPreferences("UserPrefs", 0);
        mSharedPreferencesEditor = mSharedPreferences.edit();
    }

    /**
     * Crea una instancia de SharedPreferenceUtils
     *
     * @param context contexto de la Activity o el Service
     * @return Devuelve una instancia de SharedPreferenceUtils
     */
    public static synchronized SharedPreferenceUtils getInstance(Context context) {

        if (mSharedPreferenceUtils == null) {
            mSharedPreferenceUtils = new SharedPreferenceUtils(context.getApplicationContext());
        }
        return mSharedPreferenceUtils;
    }

    /**
     * Guarda un valor en formato String
     *
     * @param key   clave de la preferencia
     * @param value valor de la clave
     */
    public void setValue(String key, String value) {
        mSharedPreferencesEditor.putString(key, value);
        mSharedPreferencesEditor.commit();
    }


    /**
     * Te devuelve el valor en formato String de la preferencia indicada por la clave
     *
     * @param key          clave de la preferencia
     * @param defaultValue valor por defecto si no se encuentra la clave
     */
    public String getValue(String key, String defaultValue) {
        return mSharedPreferences.getString(key, defaultValue);
    }

    /**
     * Borra una preferencia guardada
     *
     * @param key clave de la preferencia que se quiere borrar
     */
    public void removeKey(String key) {
        if (mSharedPreferencesEditor != null) {
            mSharedPreferencesEditor.remove(key);
            mSharedPreferencesEditor.commit();
        }
    }


    /**
     * Borra todas las preferencias guardadas
     */
    public void clear() {
        mSharedPreferencesEditor.clear().commit();
    }
}
