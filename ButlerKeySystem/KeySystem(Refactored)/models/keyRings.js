const Sequelize = require('sequelize');
const db = require('../config/database');

module.exports = db.define(
    'keyRings',{
        keyNum: {
            type: Sequelize.INTEGER,
            primaryKey: true
        },
        keyLocation:{
            type: Sequelize.STRING,
            allowNull: false
        },
        available:{
            type: Sequelize.BOOLEAN,
            allowNull: false
        }
    },
    {
        timestamps: false
    }
);