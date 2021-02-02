const Sequelize = require('sequelize');
const db = require('../config/database');

module.exports = db.define(
    'admins',{
        email: {
            type: Sequelize.STRING,
            primaryKey: true
        }
    },
    {
        timestamps: false
    }
);