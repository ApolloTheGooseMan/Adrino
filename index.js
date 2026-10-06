const express = require("express");
const mysql = require("mysql2/promise");
require("dotenv").config();

const app = express();

app.use(express.json());

const db = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_NAME,
    port: process.env.DB_PORT
});

app.post("/api/sensor", async (req, res) => {

    console.log("Received from Arduino:");
    console.log(req.body);

    const temperature = Number(req.body.temperature);
    const humidity = Number(req.body.humidity);

    if (Number.isNaN(temperature) || Number.isNaN(humidity)) {
        return res.status(400).json({
            message: "Invalid sensor data"
        });
    }

    try {

        const sql = `
            INSERT INTO sensor_readings
            (temperature, humidity)
            VALUES (?, ?)
        `;

        const [result] = await db.execute(sql, [
            temperature,
            humidity
        ]);

        console.log("Saved to database. ID:", result.insertId);

        res.json({
            message: "Sensor data received and saved"
        });

    } catch (error) {

        console.error("Database error:", error);

        res.status(500).json({
            message: "Database error"
        });
    }
});

app.listen(3000, () => {
    console.log("Server running on port 3000");
});
